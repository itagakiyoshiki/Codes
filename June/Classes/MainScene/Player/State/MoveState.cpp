#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "MoveState.h"
#include "Classes/MainScene/Player/Player.h"

using namespace SimpleMath;

void MoveState::Initialize(Player& player)
{
	player.GetPlayerMoveStruct().SetPosition(s_defaultPositon);
	m_moveStartCameraForwardVector = Vector3::Zero;
	m_moveStartCameraRightVector = Vector3::Zero;
	m_movingIndex = 0;
	m_oldMovingIndex = 0;

	m_moveNow = false;
}

void MoveState::OnEnter(Player& player, StateOrigin& beforState)
{
	m_moveNow = false;

	WeponFollowUpdate(player);

	player.GetSpear().ColliderOff();
	player.GetAxe().ColliderOff();
}

void MoveState::OnUpdate(Player& player, const DirectXTK::Camera& camera)
{
	MoveVectorUpdate(player, camera);

	PositionUpdate(player, camera);

	WeponFollowUpdate(player);

	if (InputSystem.Keyboard.wasPressedThisFrame.F)
	{
		player.GetAttackState().SetWeponState(StateStorage::WeaponState::Spear);
		player.ChangeState(StateStorage::PlayerState::Attack);
		return;
	}

	if (InputSystem.Keyboard.wasPressedThisFrame.G)
	{
		player.GetAttackState().SetWeponState(StateStorage::WeaponState::Axe);
		player.ChangeState(StateStorage::PlayerState::Attack);
		return;
	}

}

void MoveState::OnExit(Player& player, StateOrigin& nextState)
{
	m_moveNow = false;
}

void MoveState::OnCollisionEnter()
{

}

/// <summary>
/// 武器の位置を更新する
/// </summary>
/// <param name="player"></param>
void MoveState::WeponFollowUpdate(Player& player)
{
	//槍
	Vector3 _playerMatrixForward = player.GetWorldMatrix().Forward();
	_playerMatrixForward.Normalize();

	Vector3  _playerPosition = player.GetPlayerMoveStruct().GetPosition();

	Vector3 _spearPosition = ItagakiMath::Cross(-_playerMatrixForward, Vector3::Up) * s_weponDistance;
	_spearPosition += _playerPosition;
	_spearPosition.y += s_weponUpDistance;

	player.GetSpear().SetPosition(_spearPosition);
	player.GetSpear().SetTargetPosition(-_playerMatrixForward + _spearPosition);



	//斧
	Vector3 _axePosition = (ItagakiMath::Cross(Vector3::Up, _playerMatrixForward) * -s_weponDistance) + _playerPosition;
	_axePosition.y += s_weponUpDistance;

	float _axeTheta =
		std::atan2(_axePosition.z - _playerPosition.z, _axePosition.x - _playerPosition.x);

	player.GetAxe().SetAxeTheta(_axeTheta - s_axeDefaultRotationOffset);
	player.GetAxe().SetPosition(_axePosition);

}

/// <summary>
/// 移動ベクトルの更新
/// </summary>
/// <param name="player"></param>
/// <param name="camera"></param>
void MoveState::MoveVectorUpdate(Player& player, const DirectXTK::Camera& camera)
{
	m_oldMovingIndex = m_movingIndex;

	m_movingIndex = MovingState::None;
	m_moveVector = Vector3::Zero;

	Vector3 _vec = Vector3::Zero;

	if (InputSystem.Keyboard.isPressed.W)
	{
		m_movingIndex += MovingState::Move;
		_vec += m_moveStartCameraForwardVector;
	}
	if (InputSystem.Keyboard.isPressed.A)
	{
		m_movingIndex += MovingState::Move;
		_vec += -m_moveStartCameraRightVector;
	}
	if (InputSystem.Keyboard.isPressed.S)
	{
		m_movingIndex += MovingState::Move;
		_vec += -m_moveStartCameraForwardVector;
	}
	if (InputSystem.Keyboard.isPressed.D)
	{
		m_movingIndex += MovingState::Move;
		_vec += m_moveStartCameraRightVector;
	}

	_vec.Normalize();
	m_moveVector = _vec;

	if (m_oldMovingIndex == MovingState::None && m_movingIndex != MovingState::None)
	{
		player.AnimationChange(StateStorage::PlayerState::Move);
	}
	else if (m_oldMovingIndex != MovingState::None && m_movingIndex == MovingState::None)
	{
		player.AnimationChange(StateStorage::PlayerState::Stay);
	}

	//動き始めた瞬間にカメラのベクトルを保持する
	if (m_movingIndex != m_oldMovingIndex)
	{
		DirectXTK::Camera _camera = camera;
		Vector3 _cameraRotation = camera.GetRotation();
		_cameraRotation.x = 0.0f;
		_camera.SetRotation(_cameraRotation);
		m_moveStartCameraForwardVector = _camera.ForwardVector;
		m_moveStartCameraRightVector = _camera.RightVector;
	}

}

/// <summary>
/// 
/// </summary>
/// <param name="player"></param>
/// <param name="camera"></param>
void MoveState::PositionUpdate(Player& player, const DirectXTK::Camera& camera)
{
	Vector3 _nowPosition = _nowPosition = player.GetPlayerMoveStruct().GetPosition();
	_nowPosition += m_moveVector * s_speed * DXTK->Time.deltaTime;
	player.GetPlayerMoveStruct().SetPosition(_nowPosition);

}