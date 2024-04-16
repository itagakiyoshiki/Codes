//
// TitleScene.cpp
//
#include "Base/pch.h"
#include "Base/dxtk.h"
#include "../MainProject/Scenes/SceneFactory.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace SimpleMath;

// Initialize member variables.
TitleScene::TitleScene()
{

}

void TitleScene::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void TitleScene::LoadAssets()
{
	//メモリの使い方を指定する
	D3D12_HEAP_PROPERTIES _heapprop{};
	_heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;// GPUからアクセスできる設定にしてる
	// ID3D12Resource::Map()メソッドでヒープの中身をCPUから変えられる

	_heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	_heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//バッファーの性質を定義
#pragma region D3D12_RESOURCE_DESC構造体メモ
	/*
	D3D12_RESOURCE_DESC
	{
		D3D12_RESOURCE_DIMENSION Dimension; // バッファーに使うのでBUFFERを指定
		UINT64 Alignment; // 0でよい
		UINT64 Width; // 幅で全部まかなうのでsizeof(全頂点)とする
					//テクスチャの場合は画像の幅を表す
		UINT Height; // 幅で表現しているので1とする
					//テクスチャの場合は画像の高さを表す
		UINT16 DepthOrArraySize; // 1でよい
		UINT12 MipLevels; // 1でよい
		DXGI_FORMAT Format; // 画像ではないのでUNKNOWNでよい
		DXGI_SAMPLE_DESC SampleDesc; // SampleDesc.Count = 1 とする
		D3D12_TEXTURE_LAYOUT Layout; // D3D12_TEXTURE_LAYOUT_ROW_MAJOR とする
		D3D12_RESOURCE_FLAGS Flags; // NONE でよい
	}
	*/
#pragma endregion

	D3D12_RESOURCE_DESC _resdesc{};
	_resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	_resdesc.Width = sizeof(s_screenTriangleVertices);
	_resdesc.Height = 1;
	_resdesc.DepthOrArraySize = 1;
	_resdesc.MipLevels = 1;
	_resdesc.Format = DXGI_FORMAT_UNKNOWN;
	_resdesc.SampleDesc.Count = 1;
	_resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	_resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT _result;
	//GPUにメモリを作る
#pragma region CreateCommittedResourceメモ
	/*
	HRESULT CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES *pHeapProperties, // ヒープ設定構造体のアドレス
		D3D12_HEAP_FLAGS HeapFlags,	// 特に指定がないためD3D12_HEAP_FLAG_NONEでよい
		const D3D12_RESOURCE_DESC *pDesc, // リソース設定構造体のアドレス
		D3D12_RESOURCE_STATES InitialResourceState, // GPU側からは読み取り専用なので
													// D3D12_RESOURCE_STATE_GENERIC_READ
		const D3D12_CLEAR_VALUE *pOptimizedClearValue, // 使わないのでnullptrでよい
		REFIID riidResource,
		void **ppvResource
	);
	*/
#pragma endregion
	_result = DXTK->Device->CreateCommittedResource(
		&_heapprop,
		D3D12_HEAP_FLAG_NONE,
		&_resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf()));

	DX::ThrowIfFailed(_result);//エラーチェック

	//上記で作った領域にデータ書き込み
#pragma region ID3D12Resource::Map()メモ
	/*
		HRESULT Map(
			UINT Subresource,	// ミップマップなどでないため 0 でよい
			const D3D12_RANGE *pReadRange, // 範囲指定。全範囲なので nullptr でよい
			void **ppData	// 受け取るためのポインター変数のアドレス
		);
		このメソッドはバッファーの(仮想)アドレスを取得するための関数。
		CPU側でこのアドレス上のメモリに対して変更を行えば、それがGPU側に伝わるイメージをするといい
	*/
#pragma endregion
	//_map_addrに事前に作っておいた頂点データを書き込んでやることで
	//バッファー上の頂点情報を更新しています
	XMFLOAT3* _map_addr = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&_map_addr);//vertexBufferからアドレスもらい
	CopyMemory(_map_addr, s_screenTriangleVertices, sizeof(s_screenTriangleVertices));//書き込み
	m_vertexBuffer->Unmap(0, nullptr);//閉じる

#pragma region VertexBufferView メモ
	/*
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();// バッファーの仮想アドレス
	m_vertexBufferView.SizeInBytes = sizeof(s_screenTriangleVertices);// 頂点データの全バイト数
	m_vertexBufferView.StrideInBytes = sizeof(s_screenTriangleVertices[0]);// 1頂点当たりのバイト数
	*/
#pragma endregion

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = sizeof(s_screenTriangleVertices);
	m_vertexBufferView.StrideInBytes = sizeof(s_screenTriangleVertices[0]);

	_resdesc.Width = sizeof(s_screenTriangleIndecies);//リソースの幅を指定します。
	_result = DXTK->Device->CreateCommittedResource(
		&_heapprop,
		D3D12_HEAP_FLAG_NONE,
		&_resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(_result);

	m_indexBuffer->Map(0, nullptr, (void**)&_map_addr);
	CopyMemory(_map_addr, s_screenTriangleIndecies, sizeof(s_screenTriangleIndecies));
	m_indexBuffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_indexBufferView.SizeInBytes = sizeof(s_screenTriangleIndecies);
#pragma region 学校サイトメモ
	//リソースデスクリプターには、既にテクスチャのビューがあるため、
	//コンスタントバッファのビューは、その隣に配置する。
	//まず、リソースデスクリプター内の要素が2つになるため、生成時の引数を1から2に変更する。
#pragma endregion

	m_resourceDescriptors = make_unique<DescriptorHeap>(DXTK->Device, 3);

	ResourceUploadBatch _resourceUpload(DXTK->Device);
	_resourceUpload.Begin();

	auto gpu_handle = DirectXTK::CreateTextureSRV(
		DXTK->Device, L"Sprite/Title/title_bg.png",
		_resourceUpload, m_resourceDescriptors.get(), 0,
		m_texture.ReleaseAndGetAddressOf()
	);

	auto _uploadResourcesFinished = _resourceUpload.End(DXTK->CommandQueue);
	_uploadResourcesFinished.wait();

	//コンスタントバッファ
	_resdesc.Width = ((sizeof(XMMATRIX) + sizeof(XMFLOAT4)) + 0xff) & ~0xff;
	_result = DXTK->Device->CreateCommittedResource(
		&_heapprop,
		D3D12_HEAP_FLAG_NONE,
		&_resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constantBuffer.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(_result);

	//コンスタントバッファ二個目
	_resdesc.Width = ((sizeof(float)) + 0xff) & ~0xff;
	_result = DXTK->Device->CreateCommittedResource(
		&_heapprop,
		D3D12_HEAP_FLAG_NONE,
		&_resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constantBuffer2.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(_result);

	//コンスタントバッファビュー
#pragma region 学校サイトメモ
	//GetCpuHandle関数に、先頭からのインデックス(配列の添え字と同じ)を指定すると、
	//デスクリプター内の任意の要素のアドレスを取得することができる。
	//コンスタントバッファ内の変数が複数の場合、
	//同型の構造体を定義しておくとアクセスしやすくなる
#pragma endregion
	//インデックス0はテクスチャーが入っているので、
	//インデックス1にコンスタントバッファを関連付けている。
	auto _desc_addr = m_resourceDescriptors->GetCpuHandle(1);

	D3D12_CONSTANT_BUFFER_VIEW_DESC _cbv_desc{};
	_cbv_desc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
	_cbv_desc.SizeInBytes = (UINT)m_constantBuffer->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&_cbv_desc, _desc_addr);

	//コンスタントバッファビュー二個目 wipeSize用
	_desc_addr = m_resourceDescriptors->GetCpuHandle(2);

	_cbv_desc.BufferLocation = m_constantBuffer2->GetGPUVirtualAddress();
	_cbv_desc.SizeInBytes = (UINT)m_constantBuffer2->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&_cbv_desc, _desc_addr);

	//シェーダー
	ComPtr<ID3DBlob> _error_blob;//エラーが出るとここに何かが入る
	//シェーダーファイルをバイトコードに変換して最初に実行する関数をしていする
#pragma region D3DCompileFromFile メモ
	/*
	D3DCompileFromFile(
	LPCWSTR pFileName, // ファイル名(ワイド文字) wchar_t <-これ
	const D3D_SHADER_MACRO *pDefines, // シェーダーマクロオブジェクト(nullptrでよい)
	ID3DInclude *pInclude, // インクルードオブジェクト
	LPCSTR pEntrypoint, // エントリポイント(呼び出すシェーダー名)
	LPCSTR pTarget, // どのシェーダーを割り当てるか(vs,ps 等)
	UINT Flags1, // シェーダーコンパイルオプション
	UINT Flags2, // エフェクトコンパイルオプション(0 を推奨)
	ID3DBlob **ppCude, // 受け取るためのポインターのアドレス
	ID3DBlob **ppErrorMsgs // エラー用ポインターのアドレス
	);
	*/
#pragma endregion

	_result = D3DCompileFromFile(
		L"HLSL/sample.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		m_vsBlob.ReleaseAndGetAddressOf(),
		_error_blob.ReleaseAndGetAddressOf()
	);
	DX::ThrowIfFailed(_result);
	//バーテックスシェーダーとピクセルシェーダー用に二回実行する
	_result = D3DCompileFromFile(
		L"HLSL/sample.fx",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		m_psBlob.ReleaseAndGetAddressOf(),
		_error_blob.ReleaseAndGetAddressOf()
	);
	DX::ThrowIfFailed(_result);

	// ルートシグネチャ
#pragma region 学校サイトメモ
	//D3D12_DESCRIPTOR_RANGE構造体をデスクリプター内の要素分、並び順に定義する。
	//D3D12_ROOT_PARAMETER構造体のShaderVisibilityメンバは、
	//デスクリプターがどのシェーダーからアクセスするのかを設定する。
#pragma endregion


	D3D12_DESCRIPTOR_RANGE _descRange[3] = {};
	_descRange[0].NumDescriptors = 1;
	_descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	_descRange[0].BaseShaderRegister = 0;//cbuffer cb : register(b0)		- b0にあるから0
	_descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	_descRange[1].NumDescriptors = 1;
	_descRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	_descRange[1].BaseShaderRegister = 0;//cbuffer cb : register(b0)		- b0にあるから0
	_descRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	_descRange[2].NumDescriptors = 1;
	_descRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	_descRange[2].BaseShaderRegister = 1;//cbuffer WipeCB : register(b1) - b1にあるから 1
	_descRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER _rootparam{};
	_rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	_rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	_rootparam.DescriptorTable.pDescriptorRanges = _descRange;
	_rootparam.DescriptorTable.NumDescriptorRanges = 3;

#pragma region BasicShader用

	//_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//_samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	//_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	//_samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	//_samplerDesc.MinLOD = 0.0f;
	//_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//_samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion
	//sample.fx用
	D3D12_STATIC_SAMPLER_DESC _samplerDesc{};
	_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	_samplerDesc.MipLODBias = 0;
	_samplerDesc.MaxAnisotropy = 0;
	_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	_samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	_samplerDesc.MinLOD = 0.0f;
	_samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	_samplerDesc.ShaderRegister = 0;
	_samplerDesc.RegisterSpace = 0;
	_samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC _rootSignatureDesc{};
	_rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	_rootSignatureDesc.pParameters = &_rootparam;
	_rootSignatureDesc.NumParameters = 1;
	_rootSignatureDesc.pStaticSamplers = &_samplerDesc;
	_rootSignatureDesc.NumStaticSamplers = 1;

	_result = DirectX::CreateRootSignature(
		DXTK->Device,
		&_rootSignatureDesc,
		m_rootSignature.ReleaseAndGetAddressOf()
	);

	DX::ThrowIfFailed(_result);

	// パイプラインステート
#pragma region BasicShader用

	/*D3D12_GRAPHICS_PIPELINE_STATE_DESC _gpipeline{};
	_gpipeline.pRootSignature = nullptr;
	_gpipeline.VS.pShaderBytecode = m_vsBlob->GetBufferPointer();
	_gpipeline.VS.BytecodeLength = m_vsBlob->GetBufferSize();
	_gpipeline.PS.pShaderBytecode = m_psBlob->GetBufferPointer();
	_gpipeline.PS.BytecodeLength = m_psBlob->GetBufferSize();

	_gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	_gpipeline.BlendState.AlphaToCoverageEnable = false;
	_gpipeline.BlendState.IndependentBlendEnable = false;*/
#pragma endregion
	//sample.fx用
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _gpipeline = { 0 };

	_gpipeline.pRootSignature = m_rootSignature.Get();
	//シェーダーのセット
	//D3D12_SHADER_BYTECODE構造体からシェーダーのバイトコードのポインターとサイズ情報を持ってくる
	//
	_gpipeline.VS.pShaderBytecode = m_vsBlob->GetBufferPointer();
	_gpipeline.VS.BytecodeLength = m_vsBlob->GetBufferSize();
	_gpipeline.PS.pShaderBytecode = m_psBlob->GetBufferPointer();
	_gpipeline.PS.BytecodeLength = m_psBlob->GetBufferSize();

	_gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	_gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_gpipeline.DepthStencilState.DepthEnable = FALSE;
	_gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	_gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	_gpipeline.DepthStencilState.StencilEnable = FALSE;
	_gpipeline.SampleMask = UINT_MAX;
	_gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_gpipeline.NumRenderTargets = 1;
	_gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	_gpipeline.SampleDesc.Count = 1;
	_gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D12_RENDER_TARGET_BLEND_DESC _renderTargetBlendDesc{};
	_renderTargetBlendDesc.BlendEnable = false;
	_renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	_renderTargetBlendDesc.LogicOpEnable = false;

	_gpipeline.BlendState.RenderTarget[0] = _renderTargetBlendDesc;

	_gpipeline.RasterizerState.MultisampleEnable = false;
	_gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	_gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	_gpipeline.RasterizerState.DepthClipEnable = true;
	_gpipeline.RasterizerState.FrontCounterClockwise = false;
	_gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	_gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	_gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	_gpipeline.RasterizerState.AntialiasedLineEnable = false;
	_gpipeline.RasterizerState.ForcedSampleCount = 0;
	_gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	_gpipeline.DepthStencilState.DepthEnable = false;
	_gpipeline.DepthStencilState.StencilEnable = false;

	D3D12_INPUT_ELEMENT_DESC _inputLayout[] =
	{
		{
			"POSITION", // セマンティクス名
			0, // 同じセマンティクス名の時に使うインデックス(0でよい)
			DXGI_FORMAT_R32G32B32_FLOAT, // フォーマット(要素数とビット数で型を表す)
			0, // 入力スロットインデックス(0でよい)
			D3D12_APPEND_ALIGNED_ELEMENT, // データのオフセット位置(D3D12_APPEND_ALIGNED_ELEMENTでよい)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATAでよい
			0 // 一度に描画するインスタンスの数(0でよい)
		},
		{
			"TEXCOORD",
			0, DXGI_FORMAT_R32G32_FLOAT,
			0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	_gpipeline.InputLayout = { _inputLayout, _countof(_inputLayout) };

	_gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	_gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_gpipeline.NumRenderTargets = 1;
	_gpipeline.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;

	_gpipeline.SampleDesc.Count = 1;
	_gpipeline.SampleDesc.Quality = 0;


	_result = DXTK->Device->CreateGraphicsPipelineState(
		&_gpipeline, // グラフィックスパイプラインステートの各パラメータ設定
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf())
		//IID_PPV_ARGS <-これマクロです
		//インターフェイス ポインターを取得するために使用され
		// 使用されるインターフェイス ポインターの型に基づいて
		// 要求されたインターフェイスの IID 値が自動的に指定されます
	);
	DX::ThrowIfFailed(_result);

}

// Initialize a variable and audio resources.
void TitleScene::Initialize()
{
	m_camera.SetView(SimpleMath::Vector3(0.0f, 0.0f, -3.7), SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	m_camera.SetPerspectiveFieldOfView(XMConvertToRadians(30.0f), 16.0f / 16.0f, 0.1f, 10000.0f);
	//コンスタントバッファへの書き込み
#pragma region 学校サイトメモ
		//リソースを「D3D12_HEAP_TYPE_UPLOAD」で作成した場合、
	//リソースのMap関数でコンスタントバッファの仮想アドレスを取得することができる。
	//ポインタやメモリ操作関数(memcpy, CopyMemory関数など)で書き込むことができる。
#pragma endregion
	//三角形の座標などを指定している
	struct cb
	{
		XMMATRIX m_mat;//World行列
		XMFLOAT4 m_mul_color;
	} *map_buffer = nullptr;
	m_constantBuffer->Map(0, nullptr, (void**)&map_buffer);
	//ワールド行列をカメラから見た座標に変換している
	map_buffer->m_mat = SimpleMath::Matrix::Identity * m_camera.ViewMatrix * m_camera.ProjectionMatrix;
	map_buffer->m_mul_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//仮想アドレスにアクセスする必要がなくなったらUnmap関数で解放する。
	m_constantBuffer->Unmap(0, nullptr);

	float* _wipeSize;
	m_constantBuffer2->Map(0, nullptr, (void**)&_wipeSize);
	*_wipeSize = 0.0f;
	//仮想アドレスにアクセスする必要がなくなったらUnmap関数で解放する。
	m_constantBuffer2->Unmap(0, nullptr);

	m_wipeSpeed = (DXTK->Screen.Width / s_wipeTime);

	m_wipeCurrentTime = 0.0f;

	m_sceneChange = false;

	m_wipeStart = false;

	m_sceneChangeSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_sceneChangeSeFilePass);

	m_sceneChangeSeInstance = m_sceneChangeSe->CreateInstance();
}


// Releasing resources required for termination.
void TitleScene::Terminate()
{
	DXTK->AudioEngine->Reset();
	DXTK->WaitForGpu();

}

// Direct3D resource cleanup.
void TitleScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void TitleScene::OnRestartSound()
{

}

// Updates the scene.
NextScene TitleScene::Update(const float deltaTime)
{

	if (m_sceneChange)
	{
		if (m_sceneChangeSeInstance->GetState() == DirectX::PLAYING)
		{
			return NextScene::Continue;
		}

		return NextScene::MainScene;
	}

	if (InputSystem.Keyboard.wasPressedThisFrame.Space && !m_wipeStart)
	{
		m_wipeStart = true;
		m_sceneChangeSe->Play();
	}

	if (m_wipeStart)
	{
		float* _wipeSize;
		m_constantBuffer2->Map(0, nullptr, (void**)&_wipeSize);
		*_wipeSize += m_wipeSpeed * deltaTime;
		//仮想アドレスにアクセスする必要がなくなったらUnmap関数で解放する。
		m_constantBuffer2->Unmap(0, nullptr);

		m_wipeCurrentTime += deltaTime;
		if (m_wipeCurrentTime >= s_wipeTime)
		{
			m_sceneChange = true;
			m_wipeCurrentTime = 0.0f;
		}
	}

	return NextScene::Continue;
}

// Draws the scene.
void TitleScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::Black);//一度指定した色で画面をリセットする

	//テクスチャ設定
	DXTK->CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	DXTK->CommandList->SetPipelineState(m_pipelineState.Get());
	//シェーダー使わせて
	auto _heapes = m_resourceDescriptors->Heap();
	DXTK->CommandList->SetDescriptorHeaps(1, &_heapes);
	DXTK->CommandList->SetGraphicsRootDescriptorTable(0, _heapes->GetGPUDescriptorHandleForHeapStart());
	//vertexBufferViewに入っている頂点を描画
	DXTK->CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXTK->CommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	//indexBufferViewに入っている何番目の頂点を描画
	DXTK->CommandList->IASetIndexBuffer(&m_indexBufferView);
	DXTK->CommandList->DrawIndexedInstanced(12, 1, 0, 0, 0);


	DXTK->ExecuteCommandList();
}
