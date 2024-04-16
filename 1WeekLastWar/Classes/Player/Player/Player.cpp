#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Player.h"

using namespace SimpleMath;

void Player::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	m_playerModel.LoadAssets(resourceUploadBatch);

	m_bulletManeger.LoadAssets(resourceUploadBatch);

}

void Player::Initialize()
{
	m_position = m_playerMove.PositionInitialize(m_position);

	m_rotation = m_playerMove.RotationInitialize(m_rotation);

	m_worldMatrix = m_playerMove.MatrixInitialize(m_worldMatrix, m_position);

	m_bulletManeger.Initialize(m_position);

	m_shotCoolTime = 0.6f;
	m_shotCoolCurrentTime = 0.0f;

	m_colliderCenterPosition = m_position/* + Vector3(0.0f, 11.0f, -16.0f)*/;

	m_collider.Extents = s_colliderSize / 2;

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	CreateDebugModel(rtState);

	ColliderUpdate();

	m_itemGetSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_itemGetSeFilePass);
}

void Player::Update()
{
	m_position = m_playerMove.PositionUpdate(m_position);

	m_rotation = m_playerMove.RotationUpdate(m_rotation);

	m_worldMatrix = m_playerMove.MatrixUpdate(m_worldMatrix, m_position);

	m_shotCoolCurrentTime += DXTK->Time.deltaTime;
	if (m_shotCoolCurrentTime >= m_shotCoolTime)
	{
		m_bulletManeger.Fire(m_position);

		m_shotCoolCurrentTime = 0.0f;
	}

	m_bulletManeger.Update();

	ColliderUpdate();
}

void Player::ItemGet()
{
	m_itemGetSe->Play();

	m_shotCoolTime -= 0.05f;
	if (m_shotCoolTime <= 0.0f)
	{
		m_shotCoolTime = 0.1f;
	}
}

void Player::OnDeviceLost()
{
	m_playerModel.OnDeviceLost();

	m_bulletManeger.OnDeviceLost();
}

void Player::Render(DirectXTK::Camera& camera)
{
	m_playerModel.Render(camera, m_worldMatrix);

	m_bulletManeger.Render(camera);

	RenderDebugModel(camera, m_worldMatrix);
}

void Player::ColliderUpdate()
{
	Vector3 _position = m_colliderCenterPosition;

	if (InputSystem.Keyboard.isPressed.Down)
	{
		_position.z -= 10 * DXTK->Time.deltaTime;
	}
	if (InputSystem.Keyboard.isPressed.Up)
	{
		_position.y += 10 * DXTK->Time.deltaTime;
	}

	m_colliderCenterPosition = m_position;

	m_collider.Center = m_colliderCenterPosition;
}

void Player::CreateDebugModel(RenderTargetState _rtState)
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

void Player::RenderDebugModel(DirectXTK::Camera& camera, SimpleMath::Matrix wMatrix)
{
	if (!s_DebugOn)
	{
		return;
	}

	//Matrix m_worldmatrix;
	//m_worldmatrix = Matrix::CreateWorld(
	//	m_colliderCenterPosition, Vector3::Forward, Vector3::Up);
	//m_worldmatrix *= Matrix::CreateScale(1.0f);

	m_debugEffect->SetWorld(wMatrix);
	m_debugEffect->SetView(camera.ViewMatrix);
	m_debugEffect->SetProjection(camera.ViewProjectionMatrix);
	m_debugEffect->Apply(DXTK->CommandList);
	m_debugModel->Draw(DXTK->CommandList);
}
