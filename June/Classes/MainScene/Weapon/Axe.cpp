#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Axe.h"

using namespace SimpleMath;

void Axe::LoadAssets(ResourceUploadBatch& resourceUploadBatch, const bool playerUse)
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

	CreateDebugModel(rtState);
}

void Axe::Initialize()
{
	m_collider.Center = Vector3::Zero;
	m_collider.Extents = s_colliderOffSize;

	m_axeTheta = 0.0f;

	m_modelWorld = Matrix::CreateScale(s_defaultScale);

}

void Axe::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Axe::Render(DirectXTK::Camera& camera)
{

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());

	RenderDebugModel(camera);
}

/// <summary>
/// コライダーの位置情報を更新
/// </summary>
void Axe::ColliderUpdate()
{
	m_colliderCenterPosition = m_position;
	m_collider.Center = m_colliderCenterPosition;
}

/// <summary>
///	モデルの行列を更新
/// </summary>
void Axe::ModelMatrixUpdate()
{
	m_modelWorld = Matrix::CreateScale(s_defaultScale);
	m_modelWorld *= Matrix::CreateRotationX(s_defaultRotationZ);
	m_modelWorld *= Matrix::CreateRotationY(-m_axeTheta + s_defaultRotationZ);

	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;
}

/// <summary>
/// 敵の手についていくように行列を更新する関数
/// </summary>
/// <param name="parentMat"></param>
/// <param name="parentworldMat"></param>
void Axe::EnemyHandFollowUpdate(SimpleMath::Matrix parentMat, SimpleMath::Matrix parentworldMat)
{
	SimpleMath::Matrix _parentWorld = parentworldMat;
	_parentWorld.m[3][0] = 0.0f;
	_parentWorld.m[3][1] = 0.0f;
	_parentWorld.m[3][2] = 0.0f;

	m_modelWorld =
		SimpleMath::Matrix::CreateFromYawPitchRoll(
			s_defaultRotation.x, s_defaultRotation.y, s_defaultRotation.z)
		* SimpleMath::Matrix::CreateTranslation(
			s_defaultPosition.x, s_defaultPosition.y, s_defaultPosition.z)
		* parentMat
		* _parentWorld;

	m_modelWorld.m[3][0] += parentworldMat.m[3][0];
	m_modelWorld.m[3][1] += parentworldMat.m[3][1];
	m_modelWorld.m[3][2] += parentworldMat.m[3][2];

	m_position.x = m_modelWorld.m[3][0];
	m_position.y = m_modelWorld.m[3][1];
	m_position.z = m_modelWorld.m[3][2];

	ColliderUpdate();
}

/// <summary>
/// targetへ向けた行列を返す関数 
/// </summary>
/// <param name="target"></param>
Matrix Axe::ModelLookAt(Vector3 target, Vector3 position)
{
	Vector3 _y = (position - target);
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
/// コライダー確認用モデルを作成
/// </summary>
/// <param name="_rtState"></param>
void Axe::CreateDebugModel(RenderTargetState _rtState)
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
/// コライダー確認用モデルを描画
/// </summary>
/// <param name="_view"></param>
/// <param name="_proj"></param>
void Axe::RenderDebugModel(DirectXTK::Camera& camera)
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

