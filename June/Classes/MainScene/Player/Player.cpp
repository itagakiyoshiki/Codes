#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Player.h"

using namespace SimpleMath;
using namespace Microsoft::WRL;

void Player::LoadAssets(ResourceUploadBatch& resourceUploadBatch, DirectXTK::Camera& camera)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_modelFilePass, ModelLoader_IncludeBones);

	const size_t _nbones = m_model->bones.size();

	m_draw_bones = ModelBone::MakeArray(_nbones);

	m_animation_bones = ModelBone::MakeArray(_nbones);

	m_model->CopyBoneTransformsTo(_nbones, m_animation_bones.get());

	m_model->LoadStaticBuffers(device, resourceUploadBatch);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch, L"Model/Player");

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

	m_spear.LoadAssets(resourceUploadBatch, true);
	m_axe.LoadAssets(resourceUploadBatch, true);

	m_playerStateIndex = static_cast<int>(StateStorage::PlayerState::Stay);

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	ModelMatrixPositionUpdate();

	CreateDebugModel();

}

void Player::Initialize()
{

	m_collider.Center = Vector3::Zero;
	m_collider.Extents = s_colliderOnSize;

	m_moveStartcameraMatrix = Matrix::Identity;

	m_modelVector = m_modelWorld.Forward();

	m_moveStart = false;

	m_spear.Initialize();
	m_axe.Initialize();

	//現在のステートの初期化と設定
	m_moveState.Initialize(*this);

	RenderTargetState _rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);
	m_attackState.Initialize(_rtState);


	m_currentState = &m_moveState;
}

void Player::OnDeviceLost()
{
	m_modelNomal.clear();

	m_spear.OnDeviceLost();
	m_axe.OnDeviceLost();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Player::Update(const DirectXTK::Camera& camera, CounterManager& counterManager, Enemy& enemy)
{
	//一番近い敵の位置を取得し続ける
	m_nearEnemyPosition = enemy.GetPosition();

	m_currentState->OnUpdate(*this, camera);

	if (InputSystem.Keyboard.wasPressedThisFrame.Space)
	{
		counterManager.OnCounter(m_playerMoveStruct.GetPosition());
	}

	AnimationUpdate();

	ModelMatrixUpdate(camera);

	m_collider.Center = m_playerMoveStruct.GetPosition();
}

void Player::Render(DirectXTK::Camera& camera)
{
	size_t _nbones = m_model->bones.size();

	m_animation[m_playerStateIndex].Apply(*m_model, _nbones, m_draw_bones.get());

	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->DrawSkinned(
		DXTK->CommandList, _nbones, m_draw_bones.get(), m_modelWorld, m_modelNomal.cbegin());

	m_spear.Render(camera);
	m_axe.Render(camera);

	//デバッグ用モデル描画
	RenderDebugModel(camera);
}

/// <summary>
/// コライダーが当たった時に呼び出される関数
/// </summary>
void Player::OnCollisionEnter()
{
	m_hitSe->Play();
}

/// <summary>
/// 指定のステートへ切り替える
/// </summary>
/// <param name="nextState"></param>
void Player::ChangeState(StateStorage::PlayerState nextState)
{
	m_playerStateIndex = static_cast<int>(nextState);

	switch (nextState)
	{
	case StateStorage::PlayerState::Stay:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_moveState;
		m_currentState->OnEnter(*this, *m_currentState);
		AnimationChange(nextState);
		break;
	case StateStorage::PlayerState::Move:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_moveState;
		m_currentState->OnEnter(*this, *m_currentState);
		AnimationChange(nextState);
		break;
	case StateStorage::PlayerState::Attack:
		m_currentState->OnExit(*this, *m_currentState);
		m_currentState = &m_attackState;
		m_currentState->OnEnter(*this, *m_currentState);
		AnimationChange(nextState);
		break;
	default:
		break;
	}

}

/// <summary>
/// アニメーションを切り替える
/// </summary>
/// <param name="playerState"></param>
void Player::AnimationChange(StateStorage::PlayerState playerState)
{

	switch (playerState)
	{
	case StateStorage::PlayerState::Stay:
		m_playerStateIndex = static_cast<int>(StateStorage::PlayerState::Stay);
		AnimationTimeReset();
		break;
	case StateStorage::PlayerState::Move:
		m_playerStateIndex = static_cast<int>(StateStorage::PlayerState::Move);
		AnimationTimeReset();
		break;
	case StateStorage::PlayerState::Attack:
		m_playerStateIndex = static_cast<int>(StateStorage::PlayerState::Attack);
		AnimationTimeReset();
		break;
	default:
		break;
	}


}

/// <summary>
/// アニメーションの更新
/// </summary>
void Player::AnimationUpdate()
{
	m_animation[m_playerStateIndex].Update(DXTK->Time.deltaTime);
}

/// <summary>
/// アニメーションの実行時間をリセットする
/// </summary>
void Player::AnimationTimeReset()
{
	for (int i = 0; i < s_stateCount; i++)
	{
		m_animation[i].AnimTimeReset();
	}
}

/// <summary>
/// モデル行列を更新
/// </summary>
void Player::ModelMatrixUpdate(const DirectXTK::Camera& camera)
{
	ModelMatrixRotationUpdate(camera);

	ModelMatrixPositionUpdate();

	m_beforModelWorld = m_modelWorld;
}

/// <summary>
/// モデル行列の回転を更新
/// </summary>
void Player::ModelMatrixRotationUpdate(const DirectXTK::Camera& camera)
{
	m_modelWorld = Matrix::Identity;
	m_modelWorld *= Matrix::CreateScale(s_defaultScale);

	if (m_attackState.GetAttackNow())
	{
		m_modelWorld *= ModelAttackLookAt(m_nearEnemyPosition, m_playerMoveStruct.GetPosition());
		//LookAtするとモーションが丁度反対になるので180掛けて反対にする
		m_modelWorld *= Matrix::CreateRotationY(s_backRotation);
		m_moveStart = false;
		return;
	}

	if (m_moveState.GetMoveVector() != Vector3::Zero)
	{
		m_modelVector = m_moveState.GetMoveVector();
	}

	m_modelWorld *= ModelLookAt(
		m_modelVector + m_playerMoveStruct.GetPosition(),
		m_playerMoveStruct.GetPosition()
	);

}

/// <summary>
/// 入力を受け付けた時のカメラの行列を保管する変数を更新する関数
/// </summary>
/// <param name="camera"></param>
void Player::MoveStartCameraMatrixSet(const DirectXTK::Camera& camera)
{
	DirectXTK::Camera _camera = camera;
	Vector3 _cameraRotation = camera.GetRotation();
	_cameraRotation.x = 0.0f;
	_camera.SetRotation(_cameraRotation);
	m_moveStartcameraMatrix = _camera.GetViewMatrix();
}

/// <summary>
/// モデル行列の位置を更新
/// </summary>
void Player::ModelMatrixPositionUpdate()
{
	Vector3 _playerPosition = m_playerMoveStruct.GetPosition();

	m_modelWorld._41 = _playerPosition.x;
	m_modelWorld._42 = _playerPosition.y;
	m_modelWorld._43 = _playerPosition.z;
}

/// <summary>
/// 渡された行列をtargetへ向けた行列を返す関数
/// </summary>
/// <param name="target"></param>
Matrix Player::ModelLookAt(Vector3 target, Vector3 position)
{
	Vector3 _z = (target - position);
	_z.Normalize();
	Vector3 _x = ItagakiMath::Cross(Vector3::Up, _z);
	_x.Normalize();
	Vector3 _y = ItagakiMath::Cross(_z, _x);
	_y.Normalize();

	Matrix _worldMatrix = Matrix::Identity;

	_worldMatrix._11 = _x.x; _worldMatrix._12 = _y.x; _worldMatrix._13 = -_z.x;
	_worldMatrix._21 = _x.y; _worldMatrix._22 = _y.y; _worldMatrix._23 = -_z.y;
	_worldMatrix._31 = _x.z; _worldMatrix._32 = _y.z; _worldMatrix._33 = -_z.z;

	return _worldMatrix;
}

/// <summary>
/// 渡された行列をtargetへ向けた行列を返す関数 Attack用
/// </summary>
/// <param name="target"></param>
Matrix Player::ModelAttackLookAt(Vector3 target, Vector3 position)
{
	Vector3 _z = (target - position);
	_z.Normalize();
	Vector3 _x = ItagakiMath::Cross(Vector3::Up, _z);
	_x.Normalize();
	Vector3 _y = ItagakiMath::Cross(_z, _x);
	_y.Normalize();

	Matrix _worldMatrix = Matrix::Identity;

	_worldMatrix._11 = _x.x; _worldMatrix._21 = _y.x; _worldMatrix._31 = _z.x;
	_worldMatrix._12 = _x.y; _worldMatrix._22 = _y.y; _worldMatrix._32 = _z.y;
	_worldMatrix._13 = _x.z; _worldMatrix._23 = _y.z; _worldMatrix._33 = _z.z;

	return _worldMatrix;
}

//----------------------------------------------------------------------------------------
/// <summary>
/// デバッグ用モデル作成関数
/// </summary>
/// <param name="rt"></param>
void Player::CreateDebugModel()
{
	if (s_DebugOn)
	{
		RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

		EffectPipelineStateDescription _debug_pd(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			CommonStates::CullNone,
			rtState);

		m_debugModel = DirectX::GeometricPrimitive::CreateBox(s_colliderOnSize);
		m_debugEffect = std::make_unique<BasicEffect>(DXTK->Device, EffectFlags::Lighting, _debug_pd);
		m_debugEffect->EnableDefaultLighting();
	}
}

/// <summary>
/// デバッグ用モデル描画関数
/// </summary>
/// <param name="rt"></param>
void Player::RenderDebugModel(DirectXTK::Camera& camera)
{
	if (s_DebugOn)
	{
		Matrix _worldmatrix = Matrix::Identity;
		//_worldmatrix *= Matrix::CreateScale(1.0f);
		_worldmatrix *= Matrix::CreateWorld(
			m_playerMoveStruct.GetPosition(), m_modelWorld.Forward(), Vector3::Up
		);

		m_debugEffect->SetWorld(_worldmatrix);
		m_debugEffect->SetView(camera.ViewMatrix);
		m_debugEffect->SetProjection(camera.ProjectionMatrix);
		m_debugEffect->Apply(DXTK->CommandList);
		m_debugModel->Draw(DXTK->CommandList);
	}


}