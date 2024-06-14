#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Spear.h"

using namespace SimpleMath;

Spear::Spear()
{

}

void Spear::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	ModelLoad(resourceUploadBatch);
}

void Spear::Initialize()
{
	m_position = s_rightPosition;

	m_modelWorld = Matrix::Identity;

	m_rigthModeMatrix = Matrix::CreateScale(s_defaultScale) *
		Matrix::CreateRotationZ(-s_defaultRotationZ * Mathf::Deg2Rad);

	m_leftModeMatrix = Matrix::CreateScale(s_defaultScale) *
		Matrix::CreateRotationZ(s_defaultRotationZ * Mathf::Deg2Rad);

	m_attackCurrentTime = 0.0f;

	m_horizonState = HorizonState::Right;

	m_attackOk = true;
}

void Spear::OnDeviceLost()
{
	ModelOnDeviceLost();
}

void Spear::Update(SimpleMath::Vector3 position, StateStorage::MoveState movestate)
{

	FlagUpdate();

	PositionUpdate(position, movestate);

	ColliderUpdate();

}

/// <summary>
/// WeapoRackクラスから呼び出される攻撃開始処理
/// </summary>
void Spear::AttackStart()
{
	if (!m_attackOk)
	{
		return;
	}
	m_attackOk = false;
	m_attackCurrentTime = 0.0f;
}

void Spear::Render(DirectXTK::Camera& camera)
{
	RenderDebugModel(camera);
	if (m_attackOk)
	{
		return;
	}

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());

}

/// <summary>
/// 攻撃フラグを更新する関数
/// </summary>
void Spear::FlagUpdate()
{
	if (m_attackOk)
	{
		return;
	}

	m_attackCurrentTime += DXTK->Time.deltaTime;
	if (m_attackCurrentTime >= s_attackTime)
	{
		//攻撃終了処理
		m_attackOk = true;
		m_attackCurrentTime = 0.0f;
	}


}

/// <summary>
/// モデルを読み込む関数
/// </summary>
/// <param name="resourceUploadBatch"></param>
void Spear::ModelLoad(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_ModelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Models/Player/Weapons");

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

	CreateDebugModel(rtState);
}

/// <summary>
/// OnDeviceLost()で呼び出す関数
/// </summary>
void Spear::ModelOnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

/// <summary>
/// Moveの更新した位置をもとに位置を更新 自分が左右どちらを向いているのかも更新
/// </summary>
void Spear::PositionUpdate(SimpleMath::Vector3 position, StateStorage::MoveState moveState)
{
	if (moveState == StateStorage::MoveState::Left)
	{
		m_horizonState = HorizonState::Left;
	}
	else if (moveState == StateStorage::MoveState::Right)
	{
		m_horizonState = HorizonState::Right;
	}

	if (m_horizonState == HorizonState::Left)
	{
		m_modelWorld = m_leftModeMatrix;
		m_position = position + s_leftPosition;
	}
	else
	{
		m_modelWorld = m_rigthModeMatrix;
		m_position = position + s_rightPosition;
	}

	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;

}


/// <summary>
/// コライダーの位置情報を更新
/// </summary>
void Spear::ColliderUpdate()
{
	m_colliderCenterPosition = m_position;

	m_collider.Center = m_colliderCenterPosition;
}

/// <summary>
/// コライダー確認用モデルを作成
/// </summary>
/// <param name="_rtState"></param>
void Spear::CreateDebugModel(RenderTargetState _rtState)
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

/// <summary>
/// コライダー確認用モデルを描画
/// </summary>
/// <param name="_view"></param>
/// <param name="_proj"></param>
void Spear::RenderDebugModel(DirectXTK::Camera& camera)
{
	if (!s_DebugOn)
	{
		return;
	}

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
	m_debugEffect->SetProjection(camera.ViewProjectionMatrix);
	m_debugEffect->Apply(DXTK->CommandList);
	m_debugModel->Draw(DXTK->CommandList);
}
