#include "MainSceneShader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace SimpleMath;

void MainSceneShader::LoadAssets()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(s_screenTriangleVertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;
	result = DXTK->Device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(result);

	XMFLOAT3* map_addr = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&map_addr);
	CopyMemory(map_addr, s_screenTriangleVertices, sizeof(s_screenTriangleVertices));
	m_vertexBuffer->Unmap(0, nullptr);
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = sizeof(s_screenTriangleVertices);
	m_vertexBufferView.StrideInBytes = sizeof(s_screenTriangleVertices[0]);

	resdesc.Width = sizeof(s_screenTriangleIndecies);

	result = DXTK->Device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(result);

	m_indexBuffer->Map(0, nullptr, (void**)&map_addr);
	CopyMemory(map_addr, s_screenTriangleIndecies, sizeof(s_screenTriangleIndecies));
	m_indexBuffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_indexBufferView.SizeInBytes = sizeof(s_screenTriangleIndecies);

	m_resourceDescriptors = make_unique<DescriptorHeap>(DXTK->Device, 3);

	ResourceUploadBatch resourceUpload(DXTK->Device);
	resourceUpload.Begin();

	auto gpu_handle = DirectXTK::CreateTextureSRV(
		DXTK->Device, s_resultSpriteFilePass,
		resourceUpload, m_resourceDescriptors.get(), 0,
		m_texture.ReleaseAndGetAddressOf()
	);

	auto uploadResourcesFinished = resourceUpload.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();

	resdesc.Width = ((sizeof(XMMATRIX) + sizeof(XMFLOAT4)) + 0xff) & ~0xff;
	result = DXTK->Device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constantBuffer.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(result);

	resdesc.Width = ((sizeof(float)) + 0xff) & ~0xff;
	result = DXTK->Device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constantBuffer2.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(result);

	auto desc_addr = m_resourceDescriptors->GetCpuHandle(1);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc{};
	cbv_desc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
	cbv_desc.SizeInBytes = (UINT)m_constantBuffer->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&cbv_desc, desc_addr);

	//コンスタントバッファビュー二個目 wipeSize用
	desc_addr = m_resourceDescriptors->GetCpuHandle(2);

	cbv_desc.BufferLocation = m_constantBuffer2->GetGPUVirtualAddress();
	cbv_desc.SizeInBytes = (UINT)m_constantBuffer2->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&cbv_desc, desc_addr);

	ComPtr<ID3DBlob> error_blob;

	result = D3DCompileFromFile(
		L"HLSL/MainSceneFadeOut.fx",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VSMain", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		m_vsBlob.ReleaseAndGetAddressOf(),
		error_blob.ReleaseAndGetAddressOf()
	);
	DX::ThrowIfFailed(result);
	//バーテックスシェーダーとピクセルシェーダー用に二回実行する
	result = D3DCompileFromFile(
		L"HLSL/MainSceneFadeOut.fx",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		m_psBlob.ReleaseAndGetAddressOf(),
		error_blob.ReleaseAndGetAddressOf()
	);
	DX::ThrowIfFailed(result);

	D3D12_DESCRIPTOR_RANGE descRange[3] = {};
	descRange[0].NumDescriptors = 1;
	descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRange[0].BaseShaderRegister = 0;
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descRange[1].NumDescriptors = 1;
	descRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRange[1].BaseShaderRegister = 0;
	descRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descRange[2].NumDescriptors = 1;
	descRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRange[2].BaseShaderRegister = 1;
	descRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparam{};
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootparam.DescriptorTable.pDescriptorRanges = descRange;
	rootparam.DescriptorTable.NumDescriptorRanges = 3;

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;
	samplerDesc.RegisterSpace = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootparam;
	rootSignatureDesc.NumParameters = 1;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	result = DirectX::CreateRootSignature(
		DXTK->Device,
		&rootSignatureDesc,
		m_rootSignature.ReleaseAndGetAddressOf()
	);

	DX::ThrowIfFailed(result);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = { 0 };

	gpipeline.pRootSignature = m_rootSignature.Get();
	gpipeline.VS.pShaderBytecode = m_vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = m_vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = m_psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = m_psBlob->GetBufferSize();

	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthEnable = FALSE;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	gpipeline.DepthStencilState.StencilEnable = FALSE;
	gpipeline.SampleMask = UINT_MAX;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.SampleDesc.Count = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc{};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	renderTargetBlendDesc.LogicOpEnable = false;

	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	gpipeline.RasterizerState.FrontCounterClockwise = false;
	gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	gpipeline.RasterizerState.AntialiasedLineEnable = false;
	gpipeline.RasterizerState.ForcedSampleCount = 0;
	gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	gpipeline.DepthStencilState.DepthEnable = false;
	gpipeline.DepthStencilState.StencilEnable = false;

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",
			0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",
			0, DXGI_FORMAT_R32G32_FLOAT,
			0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
	gpipeline.InputLayout = { inputLayout, _countof(inputLayout) };

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;

	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;

	result = DXTK->Device->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(result);
}

void MainSceneShader::Initialize()
{
	m_camera.SetView(SimpleMath::Vector3(0.0f, 0.0f, -3.7), SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	m_camera.SetPerspectiveFieldOfView(XMConvertToRadians(30.0f), 16.0f / 16.0f, 0.1f, 10000.0f);

	struct cb
	{
		XMMATRIX mat;
		XMFLOAT4 mul_color;
	} *map_buffer = nullptr;

	m_constantBuffer->Map(0, nullptr, (void**)&map_buffer);

	map_buffer->mat = SimpleMath::Matrix::Identity * m_camera.ViewMatrix * m_camera.ProjectionMatrix;
	map_buffer->mul_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_constantBuffer->Unmap(0, nullptr);

	struct  WipeCB
	{
		float wipeSize;
		float center_x;
		float center_y;
	}*result_buffer = nullptr;

	m_constantBuffer2->Map(0, nullptr, (void**)&result_buffer);

	result_buffer->wipeSize = 100.0f;
	result_buffer->center_x = DXTK->Screen.Width / 2;
	result_buffer->center_y = DXTK->Screen.Height / 2;

	m_constantBuffer2->Unmap(0, nullptr);
}

void MainSceneShader::Terminate()
{
	DXTK->WaitForGpu();
}

void MainSceneShader::Render(float wipeSize)
{
	//0が入るなら描画しない
	if (wipeSize <= 0.0f)
	{
		return;
	}
	//1以上なら全部描画する
	//if (wipeSize >= 1.0f)
	//{
	//	//テクスチャ設定
	//	DXTK->CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	//	DXTK->CommandList->SetPipelineState(m_pipelineState.Get());
	//	//シェーダー使わせて
	//	auto heapes = m_resourceDescriptors->Heap();
	//	DXTK->CommandList->SetDescriptorHeaps(1, &heapes);
	//	DXTK->CommandList->SetGraphicsRootDescriptorTable(0, heapes->GetGPUDescriptorHandleForHeapStart());
	//	//vertexBufferViewに入っている頂点を描画
	//	DXTK->CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	DXTK->CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	//	//indexBufferViewに入っている何番目の頂点を描画
	//	DXTK->CommandList->IASetIndexBuffer(&m_indexBufferView);
	//	DXTK->CommandList->DrawIndexedInstanced(12, 1, 0, 0, 0);
	//	return;
	//}

	//コンスタントバッファに書き込み
	float* constantWipeSize;
	m_constantBuffer2->Map(0, nullptr, (void**)&constantWipeSize);
	*constantWipeSize = wipeSize;
	//仮想アドレスにアクセスする必要がなくなったらUnmap関数で解放する。
	m_constantBuffer2->Unmap(0, nullptr);

	//テクスチャ設定 
	DXTK->CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	DXTK->CommandList->SetPipelineState(m_pipelineState.Get());
	//シェーダー使わせて
	auto heapes = m_resourceDescriptors->Heap();
	DXTK->CommandList->SetDescriptorHeaps(1, &heapes);
	DXTK->CommandList->SetGraphicsRootDescriptorTable(0, heapes->GetGPUDescriptorHandleForHeapStart());
	//vertexBufferViewに入っている頂点を描画
	DXTK->CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXTK->CommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	//indexBufferViewに入っている何番目の頂点を描画
	DXTK->CommandList->IASetIndexBuffer(&m_indexBufferView);
	DXTK->CommandList->DrawIndexedInstanced(12, 1, 0, 0, 0);
}

