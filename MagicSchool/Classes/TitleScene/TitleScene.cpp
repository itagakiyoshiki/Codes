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
	//�������̎g�������w�肷��
	D3D12_HEAP_PROPERTIES _heapprop{};
	_heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;// GPU����A�N�Z�X�ł���ݒ�ɂ��Ă�
	// ID3D12Resource::Map()���\�b�h�Ńq�[�v�̒��g��CPU����ς�����

	_heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	_heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//�o�b�t�@�[�̐������`
#pragma region D3D12_RESOURCE_DESC�\���̃���
	/*
	D3D12_RESOURCE_DESC
	{
		D3D12_RESOURCE_DIMENSION Dimension; // �o�b�t�@�[�Ɏg���̂�BUFFER���w��
		UINT64 Alignment; // 0�ł悢
		UINT64 Width; // ���őS���܂��Ȃ��̂�sizeof(�S���_)�Ƃ���
					//�e�N�X�`���̏ꍇ�͉摜�̕���\��
		UINT Height; // ���ŕ\�����Ă���̂�1�Ƃ���
					//�e�N�X�`���̏ꍇ�͉摜�̍�����\��
		UINT16 DepthOrArraySize; // 1�ł悢
		UINT12 MipLevels; // 1�ł悢
		DXGI_FORMAT Format; // �摜�ł͂Ȃ��̂�UNKNOWN�ł悢
		DXGI_SAMPLE_DESC SampleDesc; // SampleDesc.Count = 1 �Ƃ���
		D3D12_TEXTURE_LAYOUT Layout; // D3D12_TEXTURE_LAYOUT_ROW_MAJOR �Ƃ���
		D3D12_RESOURCE_FLAGS Flags; // NONE �ł悢
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
	//GPU�Ƀ����������
#pragma region CreateCommittedResource����
	/*
	HRESULT CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES *pHeapProperties, // �q�[�v�ݒ�\���̂̃A�h���X
		D3D12_HEAP_FLAGS HeapFlags,	// ���Ɏw�肪�Ȃ�����D3D12_HEAP_FLAG_NONE�ł悢
		const D3D12_RESOURCE_DESC *pDesc, // ���\�[�X�ݒ�\���̂̃A�h���X
		D3D12_RESOURCE_STATES InitialResourceState, // GPU������͓ǂݎ���p�Ȃ̂�
													// D3D12_RESOURCE_STATE_GENERIC_READ
		const D3D12_CLEAR_VALUE *pOptimizedClearValue, // �g��Ȃ��̂�nullptr�ł悢
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

	DX::ThrowIfFailed(_result);//�G���[�`�F�b�N

	//��L�ō�����̈�Ƀf�[�^��������
#pragma region ID3D12Resource::Map()����
	/*
		HRESULT Map(
			UINT Subresource,	// �~�b�v�}�b�v�ȂǂłȂ����� 0 �ł悢
			const D3D12_RANGE *pReadRange, // �͈͎w��B�S�͈͂Ȃ̂� nullptr �ł悢
			void **ppData	// �󂯎�邽�߂̃|�C���^�[�ϐ��̃A�h���X
		);
		���̃��\�b�h�̓o�b�t�@�[��(���z)�A�h���X���擾���邽�߂̊֐��B
		CPU���ł��̃A�h���X��̃������ɑ΂��ĕύX���s���΁A���ꂪGPU���ɓ`���C���[�W������Ƃ���
	*/
#pragma endregion
	//_map_addr�Ɏ��O�ɍ���Ă��������_�f�[�^����������ł�邱�Ƃ�
	//�o�b�t�@�[��̒��_�����X�V���Ă��܂�
	XMFLOAT3* _map_addr = nullptr;
	m_vertexBuffer->Map(0, nullptr, (void**)&_map_addr);//vertexBuffer����A�h���X���炢
	CopyMemory(_map_addr, s_screenTriangleVertices, sizeof(s_screenTriangleVertices));//��������
	m_vertexBuffer->Unmap(0, nullptr);//����

#pragma region VertexBufferView ����
	/*
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();// �o�b�t�@�[�̉��z�A�h���X
	m_vertexBufferView.SizeInBytes = sizeof(s_screenTriangleVertices);// ���_�f�[�^�̑S�o�C�g��
	m_vertexBufferView.StrideInBytes = sizeof(s_screenTriangleVertices[0]);// 1���_������̃o�C�g��
	*/
#pragma endregion

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = sizeof(s_screenTriangleVertices);
	m_vertexBufferView.StrideInBytes = sizeof(s_screenTriangleVertices[0]);

	_resdesc.Width = sizeof(s_screenTriangleIndecies);//���\�[�X�̕����w�肵�܂��B
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
#pragma region �w�Z�T�C�g����
	//���\�[�X�f�X�N���v�^�[�ɂ́A���Ƀe�N�X�`���̃r���[�����邽�߁A
	//�R���X�^���g�o�b�t�@�̃r���[�́A���ׂ̗ɔz�u����B
	//�܂��A���\�[�X�f�X�N���v�^�[���̗v�f��2�ɂȂ邽�߁A�������̈�����1����2�ɕύX����B
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

	//�R���X�^���g�o�b�t�@
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

	//�R���X�^���g�o�b�t�@���
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

	//�R���X�^���g�o�b�t�@�r���[
#pragma region �w�Z�T�C�g����
	//GetCpuHandle�֐��ɁA�擪����̃C���f�b�N�X(�z��̓Y�����Ɠ���)���w�肷��ƁA
	//�f�X�N���v�^�[���̔C�ӂ̗v�f�̃A�h���X���擾���邱�Ƃ��ł���B
	//�R���X�^���g�o�b�t�@���̕ϐ��������̏ꍇ�A
	//���^�̍\���̂��`���Ă����ƃA�N�Z�X���₷���Ȃ�
#pragma endregion
	//�C���f�b�N�X0�̓e�N�X�`���[�������Ă���̂ŁA
	//�C���f�b�N�X1�ɃR���X�^���g�o�b�t�@���֘A�t���Ă���B
	auto _desc_addr = m_resourceDescriptors->GetCpuHandle(1);

	D3D12_CONSTANT_BUFFER_VIEW_DESC _cbv_desc{};
	_cbv_desc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
	_cbv_desc.SizeInBytes = (UINT)m_constantBuffer->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&_cbv_desc, _desc_addr);

	//�R���X�^���g�o�b�t�@�r���[��� wipeSize�p
	_desc_addr = m_resourceDescriptors->GetCpuHandle(2);

	_cbv_desc.BufferLocation = m_constantBuffer2->GetGPUVirtualAddress();
	_cbv_desc.SizeInBytes = (UINT)m_constantBuffer2->GetDesc().Width;
	DXTK->Device->CreateConstantBufferView(&_cbv_desc, _desc_addr);

	//�V�F�[�_�[
	ComPtr<ID3DBlob> _error_blob;//�G���[���o��Ƃ����ɉ���������
	//�V�F�[�_�[�t�@�C�����o�C�g�R�[�h�ɕϊ����čŏ��Ɏ��s����֐������Ă�����
#pragma region D3DCompileFromFile ����
	/*
	D3DCompileFromFile(
	LPCWSTR pFileName, // �t�@�C����(���C�h����) wchar_t <-����
	const D3D_SHADER_MACRO *pDefines, // �V�F�[�_�[�}�N���I�u�W�F�N�g(nullptr�ł悢)
	ID3DInclude *pInclude, // �C���N���[�h�I�u�W�F�N�g
	LPCSTR pEntrypoint, // �G���g���|�C���g(�Ăяo���V�F�[�_�[��)
	LPCSTR pTarget, // �ǂ̃V�F�[�_�[�����蓖�Ă邩(vs,ps ��)
	UINT Flags1, // �V�F�[�_�[�R���p�C���I�v�V����
	UINT Flags2, // �G�t�F�N�g�R���p�C���I�v�V����(0 �𐄏�)
	ID3DBlob **ppCude, // �󂯎�邽�߂̃|�C���^�[�̃A�h���X
	ID3DBlob **ppErrorMsgs // �G���[�p�|�C���^�[�̃A�h���X
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
	//�o�[�e�b�N�X�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�ɓ����s����
	_result = D3DCompileFromFile(
		L"HLSL/sample.fx",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		m_psBlob.ReleaseAndGetAddressOf(),
		_error_blob.ReleaseAndGetAddressOf()
	);
	DX::ThrowIfFailed(_result);

	// ���[�g�V�O�l�`��
#pragma region �w�Z�T�C�g����
	//D3D12_DESCRIPTOR_RANGE�\���̂��f�X�N���v�^�[���̗v�f���A���я��ɒ�`����B
	//D3D12_ROOT_PARAMETER�\���̂�ShaderVisibility�����o�́A
	//�f�X�N���v�^�[���ǂ̃V�F�[�_�[����A�N�Z�X����̂���ݒ肷��B
#pragma endregion


	D3D12_DESCRIPTOR_RANGE _descRange[3] = {};
	_descRange[0].NumDescriptors = 1;
	_descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	_descRange[0].BaseShaderRegister = 0;//cbuffer cb : register(b0)		- b0�ɂ��邩��0
	_descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	_descRange[1].NumDescriptors = 1;
	_descRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	_descRange[1].BaseShaderRegister = 0;//cbuffer cb : register(b0)		- b0�ɂ��邩��0
	_descRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	_descRange[2].NumDescriptors = 1;
	_descRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	_descRange[2].BaseShaderRegister = 1;//cbuffer WipeCB : register(b1) - b1�ɂ��邩�� 1
	_descRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER _rootparam{};
	_rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	_rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	_rootparam.DescriptorTable.pDescriptorRanges = _descRange;
	_rootparam.DescriptorTable.NumDescriptorRanges = 3;

#pragma region BasicShader�p

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
	//sample.fx�p
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

	// �p�C�v���C���X�e�[�g
#pragma region BasicShader�p

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
	//sample.fx�p
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _gpipeline = { 0 };

	_gpipeline.pRootSignature = m_rootSignature.Get();
	//�V�F�[�_�[�̃Z�b�g
	//D3D12_SHADER_BYTECODE�\���̂���V�F�[�_�[�̃o�C�g�R�[�h�̃|�C���^�[�ƃT�C�Y���������Ă���
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
			"POSITION", // �Z�}���e�B�N�X��
			0, // �����Z�}���e�B�N�X���̎��Ɏg���C���f�b�N�X(0�ł悢)
			DXGI_FORMAT_R32G32B32_FLOAT, // �t�H�[�}�b�g(�v�f���ƃr�b�g���Ō^��\��)
			0, // ���̓X���b�g�C���f�b�N�X(0�ł悢)
			D3D12_APPEND_ALIGNED_ELEMENT, // �f�[�^�̃I�t�Z�b�g�ʒu(D3D12_APPEND_ALIGNED_ELEMENT�ł悢)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�ł悢
			0 // ��x�ɕ`�悷��C���X�^���X�̐�(0�ł悢)
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
		&_gpipeline, // �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̊e�p�����[�^�ݒ�
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf())
		//IID_PPV_ARGS <-����}�N���ł�
		//�C���^�[�t�F�C�X �|�C���^�[���擾���邽�߂Ɏg�p����
		// �g�p�����C���^�[�t�F�C�X �|�C���^�[�̌^�Ɋ�Â���
		// �v�����ꂽ�C���^�[�t�F�C�X�� IID �l�������I�Ɏw�肳��܂�
	);
	DX::ThrowIfFailed(_result);

}

// Initialize a variable and audio resources.
void TitleScene::Initialize()
{
	m_camera.SetView(SimpleMath::Vector3(0.0f, 0.0f, -3.7), SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	m_camera.SetPerspectiveFieldOfView(XMConvertToRadians(30.0f), 16.0f / 16.0f, 0.1f, 10000.0f);
	//�R���X�^���g�o�b�t�@�ւ̏�������
#pragma region �w�Z�T�C�g����
		//���\�[�X���uD3D12_HEAP_TYPE_UPLOAD�v�ō쐬�����ꍇ�A
	//���\�[�X��Map�֐��ŃR���X�^���g�o�b�t�@�̉��z�A�h���X���擾���邱�Ƃ��ł���B
	//�|�C���^�⃁��������֐�(memcpy, CopyMemory�֐��Ȃ�)�ŏ������ނ��Ƃ��ł���B
#pragma endregion
	//�O�p�`�̍��W�Ȃǂ��w�肵�Ă���
	struct cb
	{
		XMMATRIX m_mat;//World�s��
		XMFLOAT4 m_mul_color;
	} *map_buffer = nullptr;
	m_constantBuffer->Map(0, nullptr, (void**)&map_buffer);
	//���[���h�s����J�������猩�����W�ɕϊ����Ă���
	map_buffer->m_mat = SimpleMath::Matrix::Identity * m_camera.ViewMatrix * m_camera.ProjectionMatrix;
	map_buffer->m_mul_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//���z�A�h���X�ɃA�N�Z�X����K�v���Ȃ��Ȃ�����Unmap�֐��ŉ������B
	m_constantBuffer->Unmap(0, nullptr);

	float* _wipeSize;
	m_constantBuffer2->Map(0, nullptr, (void**)&_wipeSize);
	*_wipeSize = 0.0f;
	//���z�A�h���X�ɃA�N�Z�X����K�v���Ȃ��Ȃ�����Unmap�֐��ŉ������B
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
		//���z�A�h���X�ɃA�N�Z�X����K�v���Ȃ��Ȃ�����Unmap�֐��ŉ������B
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
	DXTK->ClearRenderTarget(Colors::Black);//��x�w�肵���F�ŉ�ʂ����Z�b�g����

	//�e�N�X�`���ݒ�
	DXTK->CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	DXTK->CommandList->SetPipelineState(m_pipelineState.Get());
	//�V�F�[�_�[�g�킹��
	auto _heapes = m_resourceDescriptors->Heap();
	DXTK->CommandList->SetDescriptorHeaps(1, &_heapes);
	DXTK->CommandList->SetGraphicsRootDescriptorTable(0, _heapes->GetGPUDescriptorHandleForHeapStart());
	//vertexBufferView�ɓ����Ă��钸�_��`��
	DXTK->CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXTK->CommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	//indexBufferView�ɓ����Ă��鉽�Ԗڂ̒��_��`��
	DXTK->CommandList->IASetIndexBuffer(&m_indexBufferView);
	DXTK->CommandList->DrawIndexedInstanced(12, 1, 0, 0, 0);


	DXTK->ExecuteCommandList();
}
