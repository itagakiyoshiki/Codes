#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyNomalState.h"
#include "Classes/MainScene/Enemy/Enemy.h"

using namespace SimpleMath;

void EnemyNomalState::Initialize(Enemy& enemy)
{
	m_counterCurrentTime = 0.0f;

	m_counterOn = false;

	m_stateChangeOn = true;

	m_currentDistanceState = DistanceState::Out;
	m_currentMidActionState = MidActionState::None;

	m_myPosition = enemy.GetPosition();
}

void EnemyNomalState::OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
	const SimpleMath::Vector3 playerPosition)
{
	m_myPosition = enemy.GetPosition();

	m_stateChangeOn = true;
}

void EnemyNomalState::OnUpdate(Enemy& enemy,
	const SimpleMath::Vector3 playerPosition)
{

	//カウンターアニメーションの更新
	if (m_counterOn)
	{
		m_counterCurrentTime += DXTK->Time.deltaTime;
		if (m_counterCurrentTime >= s_counterTime)
		{
			m_counterCurrentTime = 0.0f;
			m_counterOn = false;

			if (enemy.GetHitOk())//攻撃ヒットアニメーションしていなければアニメーション切り替え
			{
				enemy.AnimationChange(StateStorage::EnemyState::Stay);
			}
		}
		return;
	}

	m_myPosition = enemy.GetPosition();
	m_playerPosition = playerPosition;

	DistanceStateUpdate();

	if (!enemy.GetHitOk())//攻撃ヒットアニメーションしている時は動かない
	{
		return;
	}

	switch (m_currentDistanceState)
	{
	case EnemyNomalState::DistanceState::Out:
	{
		OutUpdate(enemy);
	}
	break;
	case EnemyNomalState::DistanceState::Far:
	{
		FarUpdate(enemy);
	}
	break;
	case EnemyNomalState::DistanceState::Mid:
	{
		MidUpdate(enemy);
	}
	break;
	case EnemyNomalState::DistanceState::Near:
	{
		NearUpdate(enemy);
	}
	break;
	default:
		break;
	}

}

void EnemyNomalState::OutUpdate(Enemy& enemy)
{
	if (m_stateChangeOn)
	{
		enemy.AnimationChange(StateStorage::EnemyState::Move);
		m_stateChangeOn = false;
	}

	Vector3 _playerToVector = m_playerPosition - m_myPosition;
	_playerToVector.Normalize();
	m_myPosition += _playerToVector * s_farSpeed * DXTK->Time.deltaTime;
	enemy.SetPosition(m_myPosition);
}

void EnemyNomalState::FarUpdate(Enemy& enemy)
{
	if (m_stateChangeOn)
	{
		enemy.AnimationChange(StateStorage::EnemyState::Move);
		m_stateChangeOn = false;
	}

	Vector3 _playerToVector = m_playerPosition - m_myPosition;
	_playerToVector.Normalize();
	m_myPosition += _playerToVector * s_farSpeed * DXTK->Time.deltaTime;
	enemy.SetPosition(m_myPosition);

	//槍攻撃が来たらブロックする
	if (enemy.GetPSperOn())
	{
		enemy.ChangeState(StateStorage::EnemyState::Block);
	}
}

void EnemyNomalState::MidUpdate(Enemy& enemy)
{
	if (m_stateChangeOn)
	{
		size_t _random = ItagakiMath::Random(0, static_cast<int>(MidActionState::Count));
		switch (_random)
		{
		case 0:
			enemy.AnimationChange(StateStorage::EnemyState::WaitSeeLeft);
			m_currentMidActionState = MidActionState::Left;
			break;
		case 1:
			enemy.AnimationChange(StateStorage::EnemyState::WaitSeeRight);
			m_currentMidActionState = MidActionState::Right;
			break;
		case 2:
			enemy.GetAttackState().SetWeponState(StateStorage::WeaponState::Spear);
			enemy.ChangeState(StateStorage::EnemyState::ShotAttack);
			break;
		default:
			break;
		}

		m_stateChangeOn = false;
	}

	//様子見衛星周回
	switch (m_currentMidActionState)
	{
	case EnemyNomalState::MidActionState::Left:
	{
		Vector3 _toPlayerVector = m_myPosition - m_playerPosition;
		_toPlayerVector.y = 0.0f;
		_toPlayerVector.Normalize();

		Vector3 _crossToPlayerVector = ItagakiMath::Cross(Vector3::Up, _toPlayerVector);

		m_myPosition += _crossToPlayerVector * s_midSpeed * DXTK->Time.deltaTime;

		enemy.SetPosition(m_myPosition);
	}
	break;
	case EnemyNomalState::MidActionState::Right:
	{
		Vector3 _toPlayerVector = m_myPosition - m_playerPosition;
		_toPlayerVector.y = 0.0f;
		_toPlayerVector.Normalize();

		Vector3 _crossToPlayerVector = ItagakiMath::Cross(Vector3::Up, _toPlayerVector);

		m_myPosition += -_crossToPlayerVector * s_midSpeed * DXTK->Time.deltaTime;

		enemy.SetPosition(m_myPosition);
	}
	break;
	default:
		break;
	}

	//槍攻撃が来たらブロックする
	if (enemy.GetPSperOn())
	{
		enemy.ChangeState(StateStorage::EnemyState::Block);
	}

}

void EnemyNomalState::NearUpdate(Enemy& enemy)
{
	if (m_stateChangeOn)
	{
		m_stateChangeOn = false;
		enemy.GetAttackState().SetWeponState(StateStorage::WeaponState::Axe);
		enemy.ChangeState(StateStorage::EnemyState::HUrimawasiAttack);
	}
}

/// <summary>
/// 自分がプレイヤーとどのくらい離れているかステートとして判別する
/// </summary>
/// <param name="myPosition"></param>
void EnemyNomalState::DistanceStateUpdate()
{
	float _toPlayerDistance = ItagakiMath::Distance(m_myPosition, m_playerPosition);

	if (_toPlayerDistance >= s_farDistance)//範囲外
	{
		if (m_currentDistanceState == DistanceState::Out)
		{
			return;
		}
		m_beforeDistanceState = m_currentDistanceState;
		m_currentDistanceState = DistanceState::Out;
		m_stateChangeOn = true;
	}
	else if (_toPlayerDistance <= s_farDistance && _toPlayerDistance >= s_midDistance)//遠距離
	{
		if (m_currentDistanceState == DistanceState::Far)
		{
			return;
		}
		m_beforeDistanceState = m_currentDistanceState;
		m_currentDistanceState = DistanceState::Far;
		m_stateChangeOn = true;
	}
	else if (_toPlayerDistance <= s_midDistance && _toPlayerDistance >= s_nearDistance)//中距離
	{
		if (m_currentDistanceState == DistanceState::Mid)
		{
			return;
		}
		m_beforeDistanceState = m_currentDistanceState;
		m_currentDistanceState = DistanceState::Mid;
		m_stateChangeOn = true;
	}
	else//近距離
	{
		if (m_currentDistanceState == DistanceState::Near)
		{
			return;
		}
		m_beforeDistanceState = m_currentDistanceState;
		m_currentDistanceState = DistanceState::Near;
		m_stateChangeOn = true;
	}
}

void EnemyNomalState::OnExit(Enemy& enemy, EnemyStateOrigin& nextState)
{
	m_stateChangeOn = false;
}

void EnemyNomalState::OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider)
{
	//攻撃のヒットを受け付ける状態だったらヒットする
	if (enemy.GetHitOk())
	{
		enemy.Hit();
	}
}