#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Spear.h"

using namespace SimpleMath;

void Spear::LoadAssets(ResourceUploadBatch& resourceUploadBatch, const bool playerUse)
{
	m_playerUse = playerUse;

	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	if (m_playerUse)
	{
		m_model = DirectX::Model::CreateFromSDKMESH(
			device, s_playerVerModelFilePass);

		m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
			L"Model/Weapon");
	}
	else
	{
		m_model = DirectX::Model::CreateFromSDKMESH(
			device, s_enemyVerModelFilePass);

		m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
			L"Model/Weapon/Enemy");
	}


	m_effectFactory = DirectXTK::CreateEffectFactory(
		m_modelResources->Heap(), m_commonStates->Heap());

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullCounterClockwise,
		rtState
	);

	m_modelNomal = m_model->CreateEffects(*m_effectFactory.get(), pd, pd);

	EffectLoad();

	CreateDebugModel(rtState);
}

void Spear::Initialize()
{

	m_collider.Center = Vector3::Zero;
	m_collider.Extents = s_colliderOffSize;

	m_position = Vector3::Zero;
}

void Spear::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Spear::Render(DirectXTK::Camera& camera)
{
	RenderDebugModel(camera);

	ModelMatrixUpdate();

	EffectUpdate(camera);


	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}

/// <summary>
/// �|�W�V�������Z�b�g���A�R���C�_�[���X�V���܂�
/// </summary>
/// <param name="position"></param>
void Spear::SetPosition(const SimpleMath::Vector3 position)
{
	m_position = position;
	ColliderUpdate();
}

/// <summary>
/// ����������������w�肵�܂�
/// </summary>
/// <param name="position"></param>
void Spear::SetTargetPosition(const SimpleMath::Vector3 position)
{
	m_targetVector = m_position - position;
	m_targetVector.y = 0.0f;
	m_targetVector.Normalize();

	m_targetPosition = position + m_targetVector * s_targetVectorOffset;

	ModelMatrixUpdate();

	ColliderUpdate();
}

/// <summary>
/// �G�t�F�N�g��ǂݍ��ݏ����ݒ���s��
/// </summary>
void Spear::EffectLoad()
{
	//Effect
	DXGI_FORMAT _renderTargetFormats = DXTK->BackBufferFormat;
	m_efkRenderer = EffekseerRendererDX12::Create(
		DXTK->Device,
		DXTK->CommandQueue,
		2,
		&_renderTargetFormats,
		1,
		DXTK->DepthBufferFormat,
		false,
		10000
	);

	m_efkManager = Effekseer::Manager::Create(10000);//�ő�C���X�^���X��
	//���W�n������n�ɂ���
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	//�`��p�C���X�^���X����`��@�\��ݒ�
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	//�`��p�C���X�^���X����e�N�X�`���̓ǂݍ��݋@�\��ݒ�
	//�Ǝ��g�����\
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());

	//DirectX12���L�̏���
	m_efkMemoryPool =
		EffekseerRenderer::CreateSingleFrameMemoryPool(m_efkRenderer->GetGraphicsDevice());

	m_efkCmdList = EffekseerRenderer::CreateCommandList(
		m_efkRenderer->GetGraphicsDevice(), m_efkMemoryPool);

	m_efkRenderer->SetCommandList(m_efkCmdList);

	m_effect = Effekseer::Effect::Create(
		m_efkManager,
		(const EFK_CHAR*)L"Effect/JuneSpearEffect.efk",
		1.0f,
		(const EFK_CHAR*)L"Effect/");
}

/// <summary>
/// �G�t�F�N�g�̕`����X�V����
/// </summary>
/// <param name="camera"></param>
void Spear::EffectUpdate(DirectXTK::Camera& camera)
{
	m_efkManager->Update();//�}�l�[�W���[�̎��ԍX�V
	m_efkMemoryPool->NewFrame();//�`�悷�ׂ������_�[�^�[�Q�b�g��I��

	EffekseerRendererDX12::BeginCommandList(m_efkCmdList, DXTK->CommandList);

	m_efkRenderer->BeginRendering();//�`��O����
	m_efkManager->Draw();//�G�t�F�N�g�`��
	m_efkRenderer->EndRendering();//�`��㏈��

	EffekseerRendererDX12::EndCommandList(m_efkCmdList);

	Effekseer::Matrix44 _fkViewMat;
	Effekseer::Matrix44 _fkProjMat;
	auto _view = camera.ViewMatrix;
	auto _proj = camera.ProjectionMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_fkViewMat.Values[i][j] = _view.r[i].m128_f32[j];
			_fkProjMat.Values[i][j] = _proj.r[i].m128_f32[j];
		}
	}

	m_efkRenderer->SetCameraMatrix(_fkViewMat);
	m_efkRenderer->SetProjectionMatrix(_fkProjMat);
}

/// <summary>
/// �R���C�_�[�ɓ����������ɌĂ΂��֐�
/// </summary>
void Spear::OnCollisionEnter()
{
	m_efkHandle = m_efkManager->Play(m_effect, m_position.x, m_position.y, m_position.z);
	m_efkManager->SetScale(m_efkHandle, s_effectScale, s_effectScale, s_effectScale);
}

/// <summary>
/// �R���C�_�[�̈ʒu�����X�V
/// </summary>
void Spear::ColliderUpdate()
{
	m_colliderCenterPosition = m_position;

	Vector3 _forwordVector = m_modelWorld.Up();

	m_colliderCenterPosition = m_colliderCenterPosition + _forwordVector * 100.0f;

	m_collider.Center = m_colliderCenterPosition;
}

/// <summary>
///	���f���̍s����X�V
/// </summary>
void Spear::ModelMatrixUpdate()
{
	m_modelWorld = Matrix::CreateScale(s_defaultScale);

	m_modelWorld *= ModelLookAt(m_targetPosition, m_position);

	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;
}

/// <summary>
/// �n���ꂽ�s���target�֌������s���Ԃ��֐� 
/// </summary>
/// <param name="target"></param>
Matrix Spear::ModelLookAt(Vector3 target, Vector3 position)
{
	Vector3 _targetPosition = target;
	_targetPosition.y = 0.0f;
	Vector3 _myPosition = position;
	_myPosition.y = 0.0f;

	Vector3 _y = (_targetPosition - _myPosition);
	_y.Normalize();

	Vector3 _x = ItagakiMath::Cross(Vector3::Up, _y);
	_x.Normalize();

	Vector3 _z = ItagakiMath::Cross(_y, _x);
	_z.Normalize();


	Matrix _worldMatrix = Matrix::Identity;

	_worldMatrix._11 = _x.x; _worldMatrix._21 = _y.x; _worldMatrix._31 = _z.x;
	_worldMatrix._12 = _x.y; _worldMatrix._22 = _y.y; _worldMatrix._32 = _z.y;
	_worldMatrix._13 = _x.z; _worldMatrix._23 = _y.z; _worldMatrix._33 = _z.z;

	return _worldMatrix;
}


/// <summary>
/// �R���C�_�[�m�F�p���f�����쐬
/// </summary>
/// <param name="_rtState"></param>
void Spear::CreateDebugModel(RenderTargetState _rtState)
{
	if (s_DebugOn)
	{
		EffectPipelineStateDescription _debug_pd(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			CommonStates::CullNone,
			_rtState);

		m_debugModel = DirectX::GeometricPrimitive::CreateBox(s_colliderOnSize / 2);
		m_debugEffect = std::make_unique<BasicEffect>(DXTK->Device, EffectFlags::Lighting, _debug_pd);
		m_debugEffect->EnableDefaultLighting();
	}
}

/// <summary>
/// �R���C�_�[�m�F�p���f����`��
/// </summary>
/// <param name="_view"></param>
/// <param name="_proj"></param>
void Spear::RenderDebugModel(DirectXTK::Camera& camera)
{
	if (s_DebugOn)
	{
		Matrix _worldmatrix = Matrix::Identity;
		_worldmatrix *= Matrix::CreateScale(1.0f);
		_worldmatrix *= Matrix::CreateWorld(
			m_colliderCenterPosition, Vector3::Forward, Vector3::Up
		);

		_worldmatrix._41 = m_colliderCenterPosition.x;
		_worldmatrix._42 = m_colliderCenterPosition.y;
		_worldmatrix._43 = m_colliderCenterPosition.z;

		m_debugEffect->SetWorld(_worldmatrix);
		m_debugEffect->SetView(camera.ViewMatrix);
		m_debugEffect->SetProjection(camera.ProjectionMatrix);
		m_debugEffect->Apply(DXTK->CommandList);
		m_debugModel->Draw(DXTK->CommandList);
	}
}

