#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Enemy.h"

using namespace SimpleMath;

void Enemy::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_ModelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Models/Enemy");

	m_effectFactory = DirectXTK::CreateEffectFactory(
		m_modelResources->Heap(), m_commonStates->Heap());

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState
	);

	m_modelNomal = m_model->CreateEffects(*m_effectFactory.get(), pd, pd);

	m_colliderCenterPosition = m_enemyStruct.GetPosition();

	DirectX::BoundingBox _box = m_enemyStruct.GetCollider();
	_box.Extents = s_colliderSize / 2;
	m_enemyStruct.SetCollider(_box);

	CreateDebugModel(rtState);
}

void Enemy::Initialize()
{
	m_rotation = Vector3(0, 0, 0);

	m_worldMatrix = Matrix::Identity;

	m_worldMatrix *= Matrix::CreateScale(s_modelScale);
	m_worldMatrix *= Matrix::CreateRotationX(s_modelDefaultRotation * Mathf::Deg2Rad);

	m_enemyStruct.SetMaxHP(s_defaultMaxHp);
	m_enemyStruct.SetHP(m_enemyStruct.GetMaxHP());

	m_enemyStruct.InGameOff();

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);
}

void Enemy::Launch(SimpleMath::Vector3 position)
{
	m_enemyStruct.InGameOn();

	m_enemyStruct.SetPosition(position);
}

void Enemy::Update()
{

	if (m_enemyStruct.GetInGame())
	{
		Vector3 _position = m_enemyStruct.GetPosition();
		if (_position.z <= s_screenOutPosiiton)
		{
			m_enemyStruct.SetHP(m_enemyStruct.GetMaxHP());
			_position = s_offPositon;
			m_enemyStruct.SetPosition(_position);
			m_enemyStruct.InGameOff();
		}
		else
		{
			_position.z -= s_speed * DXTK->Time.deltaTime;
		}
		m_enemyStruct.SetPosition(_position);
	}

	ColliderUpdate();

	MatrixUpdate();
}

void Enemy::Hit()
{
	m_hitSe->Play();
	m_enemyStruct.MinusHP();
	if (m_enemyStruct.GetHP() != 0)
	{
		return;
	}

	m_enemyStruct.PlusMaxHP();
	m_enemyStruct.SetHP(m_enemyStruct.GetMaxHP());
	Vector3 _position = m_enemyStruct.GetPosition();
	_position = s_offPositon;
	m_enemyStruct.SetPosition(_position);
	m_enemyStruct.InGameOff();
}

void Enemy::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Enemy::Render(DirectXTK::Camera& camera)
{

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_worldMatrix,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());

	RenderDebugModel(camera.ViewMatrix, camera.ProjectionMatrix);
}

void Enemy::ColliderUpdate()
{
	m_colliderCenterPosition = m_enemyStruct.GetPosition();

	DirectX::BoundingBox _box = m_enemyStruct.GetCollider();
	_box.Center = m_colliderCenterPosition;
	m_enemyStruct.SetCollider(_box);
}

void Enemy::MatrixUpdate()
{
	m_worldMatrix._41 = m_enemyStruct.GetPosition().x;
	m_worldMatrix._42 = m_enemyStruct.GetPosition().y;
	m_worldMatrix._43 = m_enemyStruct.GetPosition().z;
}

void Enemy::CreateDebugModel(RenderTargetState _rtState)
{
	if (!s_DebugOn)
	{
		return;
	}

	EffectPipelineStateDescription _debug_pd(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		_rtState);

	m_debugModel = DirectX::GeometricPrimitive::CreateBox(s_colliderSize / 2);
	m_debugEffect = std::make_unique<BasicEffect>(DXTK->Device, EffectFlags::Lighting, _debug_pd);
	m_debugEffect->EnableDefaultLighting();
}

void Enemy::RenderDebugModel(SimpleMath::Matrix _view, SimpleMath::Matrix _proj)
{
	if (!s_DebugOn)
	{
		return;
	}

	Matrix m_worldmatrix;
	m_worldmatrix = Matrix::CreateScale(1.0f);
	m_worldmatrix *= Matrix::CreateWorld(
		m_colliderCenterPosition, Vector3::Forward, Vector3::Up
	);
	m_debugEffect->SetWorld(m_worldmatrix);
	m_debugEffect->SetView(_view);
	m_debugEffect->SetProjection(_proj);
	m_debugEffect->Apply(DXTK->CommandList);
	m_debugModel->Draw(DXTK->CommandList);
}