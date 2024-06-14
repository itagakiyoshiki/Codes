#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "AttackState.h"
#include "Classes/MainScene/Player/Player.h"

using namespace SimpleMath;

void AttackState::Initialize(RenderTargetState rt)
{
	m_animationCurrentTime = 0.0f;

	m_axeTheta = 0.0f;

	m_spearOn = false;

	m_attackNow = false;

	m_weponLaunchOn = false;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	m_weponState = StateStorage::WeaponState::None;
	m_nextWeponState = StateStorage::WeaponState::None;

}

void AttackState::OnEnter(Player& player, StateOrigin& beforState)
{
	m_animationCurrentTime = 0.0f;

	m_attackNow = true;

	m_spearOn = false;

	m_nextWeponState = StateStorage::WeaponState::None;

	player.GetSpear().ColliderOff();
	player.GetAxe().ColliderOff();

	//プレイヤーの向きを更新
	Matrix _modelWorld = Matrix::Identity;
	_modelWorld *= Matrix::CreateScale(s_defaultScale);

	_modelWorld *= ModelAttackLookAt(player.GetMostNearEnemyPosition(), player.GetPlayerMoveStruct().GetPosition());
	//LookAtするとモーションが丁度反対になるので180掛けて反対にする
	_modelWorld *= Matrix::CreateRotationY(s_backRotation);

	Vector3 _playerMatrixForward = _modelWorld.Forward();
	Vector3 _enemyPosition = player.GetMostNearEnemyPosition();
	Vector3 _playerPosition = player.GetPlayerMoveStruct().GetPosition();

	switch (m_weponState)
	{
	case StateStorage::WeaponState::Spear:
	{
		//使ってない方も向きを整える
		Vector3 _axePosition = (ItagakiMath::Cross(Vector3::Up, _playerMatrixForward) * -s_weponDistance) + _playerPosition;
		_axePosition.y += s_weponUpDistance;

		float _axeTheta =
			std::atan2(_axePosition.z - _playerPosition.z, _axePosition.x - _playerPosition.x);

		player.GetAxe().SetAxeTheta(_axeTheta - s_axeDefaultRotationOffset);
		player.GetAxe().SetPosition(_axePosition);

		m_weponLaunchOn = false;

		Vector3 _toEnemyVector = _enemyPosition - _playerPosition;
		_toEnemyVector.y = 0.0f;
		_toEnemyVector.Normalize();

		m_weaponPosition = (ItagakiMath::Cross(Vector3::Up, _toEnemyVector) * s_weponDistance) + _playerPosition;
		m_weaponPosition.y += s_weponUpDistance;

		player.GetSpear().SetPosition(m_weaponPosition);
		player.GetSpear().SetTargetPosition(_enemyPosition);

	}
	break;
	case StateStorage::WeaponState::Axe:
	{
		//使ってない方も向きを整える
		Vector3 _spearPosition = ItagakiMath::Cross(-_playerMatrixForward, Vector3::Up) * s_weponDistance;
		_spearPosition += _playerPosition;
		_spearPosition.y += s_weponUpDistance;

		player.GetSpear().SetPosition(_spearPosition);
		player.GetSpear().SetTargetPosition(-_playerMatrixForward + _spearPosition);
		m_weponLaunchOn = false;

		Vector3 _toEnemyVector = _enemyPosition - _playerPosition;
		_toEnemyVector.y = 0.0f;
		_toEnemyVector.Normalize();

		m_weaponPosition = (ItagakiMath::Cross(Vector3::Up, _toEnemyVector) * -s_weponDistance) + _playerPosition;
		m_weaponPosition.y += s_weponUpDistance;

		//周回の初期角度を設定
		m_axeTheta =
			std::atan2(m_weaponPosition.z - _playerPosition.z, m_weaponPosition.x - _playerPosition.x);

		//半周するのでPIを設定し初期角度分足しておく
		m_axeEndTheta = m_axeTheta + -Mathf::PI;

		player.GetAxe().SetAxeTheta(m_axeTheta);
		player.GetAxe().SetPosition(m_weaponPosition);
		player.GetAxe().ColliderOn();
	}
	break;
	case StateStorage::WeaponState::Count:
		break;
	default:
		break;
	}



}

void AttackState::OnUpdate(Player& player, const DirectXTK::Camera& camera)
{

	m_animationCurrentTime += DXTK->Time.deltaTime;

	switch (m_weponState)
	{
	case StateStorage::WeaponState::None:
		break;
	case StateStorage::WeaponState::Spear:
	{
		if (m_animationCurrentTime >= s_weponLaunchTime && !m_weponLaunchOn)
		{
			m_weponLaunchOn = true;
			m_spearOn = true;

			m_spearTargetVector = player.GetMostNearEnemyPosition() - m_weaponPosition;
			m_spearTargetVector.y = 0.0f;
			m_spearTargetVector.Normalize();
			player.GetSpear().SetTargetPosition(player.GetMostNearEnemyPosition());
			player.GetSpear().ColliderOn();
		}

		if (m_weponLaunchOn)
		{
			m_weaponPosition = m_weaponPosition + m_spearTargetVector * s_spearLaunchSpeed;

			player.GetSpear().SetPosition(m_weaponPosition);
		}
	}
	break;
	case StateStorage::WeaponState::Axe:
	{

		if (m_animationCurrentTime >= s_weponLaunchTime && !m_weponLaunchOn)
		{
			m_weponLaunchOn = true;
		}

		if (m_weponLaunchOn)
		{

			m_axeTheta += -s_axeLaunchSpeed * DXTK->Time.deltaTime * Mathf::Deg2Rad;
			if (m_axeTheta <= m_axeEndTheta)
			{
				m_axeTheta = m_axeEndTheta;
			}

			m_weaponPosition.x =
				player.GetPlayerMoveStruct().GetPosition().x +
				(cos(m_axeTheta) * s_weponUpDistance);

			m_weaponPosition.z =
				player.GetPlayerMoveStruct().GetPosition().z +
				(sin(m_axeTheta) * s_weponUpDistance);

			player.GetAxe().SetAxeTheta(m_axeTheta);
			player.GetAxe().SetPosition(m_weaponPosition);
		}
	}
	break;
	case StateStorage::WeaponState::Count:
		break;
	default:
		break;
	}

	//コンボを受け付ける
	if (m_animationCurrentTime >= s_comboInputTime)
	{
		if (InputSystem.Keyboard.wasPressedThisFrame.F)
		{
			m_nextWeponState = StateStorage::WeaponState::Spear;
		}

		if (InputSystem.Keyboard.wasPressedThisFrame.G)
		{
			m_nextWeponState = StateStorage::WeaponState::Axe;
		}
	}

	//攻撃アニメーション終わったらステート変更
	if (m_animationCurrentTime >= s_animationTime)
	{
		switch (m_nextWeponState)
		{
		case StateStorage::WeaponState::None:
			player.ChangeState(StateStorage::PlayerState::Stay);
			break;
		case StateStorage::WeaponState::Spear:
			player.GetAttackState().SetWeponState(StateStorage::WeaponState::Spear);
			player.ChangeState(StateStorage::PlayerState::Attack);
			break;
		case StateStorage::WeaponState::Axe:
			player.GetAttackState().SetWeponState(StateStorage::WeaponState::Axe);
			player.ChangeState(StateStorage::PlayerState::Attack);
			break;
		default:
			break;
		}

	}
}

void AttackState::OnExit(Player& player, StateOrigin& nextState)
{
	m_attackNow = false;

	m_weponLaunchOn = false;

	m_spearOn = false;

	player.GetSpear().ColliderOff();
	player.GetAxe().ColliderOff();

}

/// <summary>
/// コライダーが当たった時に呼び出される関数
/// </summary>
void AttackState::OnCollisionEnter()
{
	m_hitSe->Play();
}

/// <summary>
/// 渡された行列をtargetへ向けた行列を返す関数 Attack用
/// </summary>
/// <param name="target"></param>
Matrix AttackState::ModelAttackLookAt(Vector3 target, Vector3 position)
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
