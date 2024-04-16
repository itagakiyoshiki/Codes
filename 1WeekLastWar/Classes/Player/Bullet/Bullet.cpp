#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Bullet.h"

using namespace SimpleMath;

void Bullet::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromCMO(
		device, s_ModelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Models/Player/Bullet");

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

	CreateDebugModel(rtState);
}

void Bullet::Initialize(SimpleMath::Vector3 postion)
{
	m_worldMatrix = Matrix::Identity;

	m_bulletStruct.SetPosition(s_offPositon);

	m_rePopCurrentTime = 0.0f;

	m_bulletStruct.SetFireOnFlag(false);

	m_colliderCenterPosition = m_bulletStruct.GetPosition() + Vector3(0.0f, 0.1f, 0.0f);

	DirectX::BoundingBox _box = m_bulletStruct.GetCollider();
	_box.Extents = s_colliderSize / 2;
	m_bulletStruct.SetCollider(_box);

	ColliderUpdate();

	m_shotSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_shotSeFilePass);

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);
}

void Bullet::Update()
{
	if (!m_bulletStruct.GetFireOn())
	{
		return;
	}

	Vector3 _nowPositon = m_bulletStruct.GetPosition();
	_nowPositon.z += 10 * DXTK->Time.deltaTime;
	m_bulletStruct.SetPosition(_nowPositon);

	m_rePopCurrentTime += DXTK->Time.deltaTime;

	if (m_rePopCurrentTime >= s_rePopTime)
	{
		m_bulletStruct.SetPosition(s_offPositon);

		m_rePopCurrentTime = 0.0f;

		m_bulletStruct.SetFireOnFlag(false);

		ColliderUpdate();
	}

	ColliderUpdate();
}

void Bullet::Fire(SimpleMath::Vector3 postion)
{
	m_bulletStruct.SetPosition(postion);

	m_shotSe->Play();

	m_bulletStruct.SetFireOnFlag(true);

}

void Bullet::Hit()
{
	m_bulletStruct.SetPosition(s_offPositon);

	m_hitSe->Play();

	m_rePopCurrentTime = 0.0f;

	m_bulletStruct.SetFireOnFlag(false);

	ColliderUpdate();
}

void Bullet::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Bullet::Render(DirectXTK::Camera& camera)
{

	MatrixUpdate();

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_worldMatrix,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());

	RenderDebugModel(camera.ViewMatrix, camera.ProjectionMatrix);
}

void Bullet::ColliderUpdate()
{
	m_colliderCenterPosition = m_bulletStruct.GetPosition() + Vector3(0.0f, 0.1f, 0.0f);

	DirectX::BoundingBox _box = m_bulletStruct.GetCollider();
	_box.Center = m_colliderCenterPosition;
	m_bulletStruct.SetCollider(_box);
}

void Bullet::MatrixUpdate()
{
	m_worldMatrix._41 = m_bulletStruct.GetPosition().x;
	m_worldMatrix._42 = m_bulletStruct.GetPosition().y;
	m_worldMatrix._43 = m_bulletStruct.GetPosition().z;
}

void Bullet::CreateDebugModel(RenderTargetState _rtState)
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

void Bullet::RenderDebugModel(SimpleMath::Matrix _view, SimpleMath::Matrix _proj)
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