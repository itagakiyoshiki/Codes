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
		device, s_modelFilePass, ModelLoader_IncludeBones);

	m_model->LoadStaticBuffers(device, resourceUploadBatch);

	const size_t nbones = m_model->bones.size();
	m_draw_bones = ModelBone::MakeArray(nbones);
	m_animation_bones = ModelBone::MakeArray(nbones);
	m_model->CopyBoneTransformsTo(nbones, m_animation_bones.get());

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch, L"Model/Enemy");

	for (int i = 0; i < s_stateCount; i++)
	{
		DX::ThrowIfFailed(m_animation[i].Load(s_animationFilePassArry[i]));

		m_animation[i].Bind(*m_model);

	}

	m_effectFactory = DirectXTK::CreateEffectFactory(
		m_modelResources->Heap(), m_commonStates->Heap());

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState
	);

	m_modelNomal = m_model->CreateEffects(*m_effectFactory.get(), pd, pd);

	m_spear.LoadAssets(resourceUploadBatch, false);
	m_axe.LoadAssets(resourceUploadBatch, false);

	CreateDebugModel(rtState);

	uint32_t index = 0;
	for (const auto& it : m_model->bones)
	{
		if (it.name == L"mixamorig:RightHand")
		{
			int i = 0;
			m_handBoneIndex = index;
			break;
		}

		++index;
	}


}

void Enemy::Initialize(Vector3 position)
{
	m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Stay);

	m_spear.Initialize();
	m_axe.Initialize();

	//現在のステートの初期化と設定
	m_nomalState.Initialize(*this);
	m_attackState.Initialize(*this);
	m_knockBackState.Initialize(*this);
	m_blockState.Initialize(*this);

	m_currentState = &m_nomalState;

	m_position = position;

	ColliderUpdate();
	m_collider.Extents = s_colliderSize / 2;

	m_defaultSizeMatrix =
		SimpleMath::Matrix::CreateScale(s_defaultScale) *
		SimpleMath::Matrix::CreateRotationX(s_defaultRotation);

	m_reactioningSizeMatrix =
		SimpleMath::Matrix::CreateScale(s_hitReactionMaxScale) *
		SimpleMath::Matrix::CreateRotationX(s_defaultRotation);

	//行列の初期化
	m_modelWorld = Matrix::Identity;
	m_modelWorld *= Matrix::CreateScale(s_defaultScale);

	m_knockBackCurrentCount = 0;

	m_hitCoolCurrentTime = 0.0f;

	m_reactionCoolCurrentTime = 0.0f;

	m_attackCoolCurrentTime = 0.0f;

	m_hitOk = true;

	m_attackOk = true;

	m_reactionNowBigger = false;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);


}

void Enemy::OnDeviceLost()
{
	m_modelNomal.clear();

	m_spear.OnDeviceLost();
	m_axe.OnDeviceLost();
	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Enemy::Update(const SimpleMath::Vector3 playerPosition,
	DirectXTK::Camera& camera,
	const bool& sperOn)
{
	m_sperOn = sperOn;

	m_playerPosition = playerPosition;

	m_currentState->OnUpdate(*this, playerPosition);

	ColliderUpdate();

}

void Enemy::Render(DirectXTK::Camera& camera)
{

	AnimationUpdate();

	//モデルをプレイヤーの位置に向ける
	m_modelWorld = ModelLookAt(m_modelWorld, m_nomalState.GetPlayerPosition(), m_position);

	HitReactionUpdate();

	MatrixUpdate();

	size_t _nbones = m_model->bones.size();

	m_animation[m_enemyStateIndex].Apply(*m_model, _nbones, m_draw_bones.get());

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->DrawSkinned(
		DXTK->CommandList, _nbones, m_draw_bones.get(), m_modelWorld, m_modelNomal.cbegin());


	RenderDebugModel(camera);

	m_spear.Render(camera);

	m_axe.EnemyHandFollowUpdate(m_draw_bones[m_handBoneIndex], m_modelWorld);
	m_axe.Render(camera);

}

/// <summary>
/// 攻撃リアクションを更新する
/// </summary>
void Enemy::HitReactionUpdate()
{
	if (m_hitOk)
	{
		m_modelWorld *= Matrix::CreateScale(s_defaultScale);
		return;
	}

	//一定時間を超過したら、攻撃受付開始
	m_hitCoolCurrentTime += DXTK->Time.deltaTime;
	if (m_hitCoolCurrentTime >= s_hitCoolTIme)
	{
		m_modelWorld *= Matrix::CreateScale(s_defaultScale);

		m_hitCoolCurrentTime = 0.0f;

		m_reactionCoolCurrentTime = 0.0f;

		m_hitOk = true;

		m_reactionNowBigger = false;
	}

}

/// <summary>
/// 攻撃が当たった時の処理
/// </summary>
void Enemy::OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider)
{
	m_currentState->OnCollisionEnter(enemy, collider);

	if (m_hitOk)
	{
		return;
	}

	if (m_knockBackCurrentCount >= s_knockBackCount)
	{
		m_hitOk = false;
		m_nomalState.SetCounterOn(false);
		m_knockBackCurrentCount = 0;
		ChangeState(StateStorage::EnemyState::NockBack);
	}
	else
	{
		ChangeState(StateStorage::EnemyState::Hit);
	}
}


/// <summary>
/// アニメーションの更新
/// </summary>
void Enemy::AnimationUpdate()
{
	m_animation[m_enemyStateIndex].Update(DXTK->Time.deltaTime * 1.0f);
}

/// <summary>
/// アニメーションの実行時間をリセットする
/// </summary>
void Enemy::AnimationTimeReset()
{
	for (int i = 0; i < s_stateCount; i++)
	{
		m_animation[i].AnimTimeReset();
	}
}

/// <summary>
/// ワールド行列の位置情報を更新
/// </summary>
void Enemy::MatrixUpdate()
{
	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;
}

/// <summary>
/// コライダーの位置情報を更新
/// </summary>
void Enemy::ColliderUpdate()
{
	m_colliderCenterPosition = m_position;

	m_collider.Center = m_colliderCenterPosition;
}

/// <summary>
/// ステート切り替え関数
/// </summary>
/// <param name="nextState"></param>
void Enemy::ChangeState(StateStorage::EnemyState nextState)
{

	switch (nextState)
	{
	case StateStorage::EnemyState::Stay:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_nomalState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::Hit:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_nomalState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::Counter:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_nomalState;
		m_nomalState.CounterOn();//カウンタされたとNormalステートに伝える
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::Move:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_nomalState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::HUrimawasiAttack:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_attackState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::NockBack:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_knockBackState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::ShotAttack://shot攻撃をすると伝える
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_attackState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	case StateStorage::EnemyState::Block:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_blockState;
		m_currentState->OnEnter(*this, *m_currentState, m_playerPosition);
		AnimationChange(nextState);
		break;
	default:
		break;
	}


}

/// <summary>
/// アニメーション切り替え切り替え関数
/// </summary>
/// <param name="nextState"></param>
void Enemy::AnimationChange(StateStorage::EnemyState nextState)
{
	////今と同じアニメーションならアニメーション切り替え処理を行わない
	//if (static_cast<int>(nextState) != m_enemyStateIndex)
	//{
	AnimationTimeReset();

	switch (nextState)
	{
	case StateStorage::EnemyState::Stay:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Stay);
		break;
	case StateStorage::EnemyState::Move:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Move);
		break;
	case StateStorage::EnemyState::HUrimawasiAttack:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::HUrimawasiAttack);
		break;
	case StateStorage::EnemyState::Hit:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Hit);
		break;
	case StateStorage::EnemyState::NockBack:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::NockBack);
		break;
	case StateStorage::EnemyState::WaitSeeRight:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::WaitSeeRight);
		break;
	case StateStorage::EnemyState::WaitSeeLeft:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::WaitSeeLeft);
		break;
	case StateStorage::EnemyState::ShotAttack:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::ShotAttack);
		break;
	case StateStorage::EnemyState::Counter:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Counter);
		break;
	case StateStorage::EnemyState::Block:
		m_enemyStateIndex = static_cast<int>(StateStorage::EnemyState::Block);
		break;
	default:
		break;
	}


	//}
}

/// <summary>
/// 渡された行列をtargetへ向けた行列を返す関数
/// </summary>
/// <param name="target"></param>
Matrix Enemy::ModelLookAt(Matrix matrix, Vector3 target, Vector3 position)
{
	Vector3 _z = (position - target);
	_z.Normalize();
	Vector3 _x = Cross(Vector3::Up, _z);
	_x.Normalize();
	Vector3 _y = Cross(_z, _x);
	_y.Normalize();

	Matrix _matrix = Matrix::Identity;

	_matrix._11 = _x.x; _matrix._21 = _y.x; _matrix._31 = _z.x;
	_matrix._12 = _x.y; _matrix._22 = _y.y; _matrix._32 = _z.y;
	_matrix._13 = _x.z; _matrix._23 = _y.z; _matrix._33 = _z.z;

	return _matrix;
}

/// <summary>
/// 与えられた座標同士の外積を返す
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Enemy::Cross(Vector3 v1, Vector3 v2) {
	return Vector3((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

//-----------デバック-----------------------------------------------------------------------------
/// <summary>
/// コライダー確認用モデルを作成
/// </summary>
/// <param name="_rtState"></param>
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

/// <summary>
/// コライダー確認用モデルを描画
/// </summary>
/// <param name="_view"></param>
/// <param name="_proj"></param>
void Enemy::RenderDebugModel(DirectXTK::Camera& camera)
{
	if (!s_DebugOn)
	{
		return;
	}



}
