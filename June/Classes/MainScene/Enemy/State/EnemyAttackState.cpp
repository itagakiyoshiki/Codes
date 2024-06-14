#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyAttackState.h"
#include "Classes/MainScene/Enemy/Enemy.h"

using namespace SimpleMath;

void EnemyAttackState::Initialize(Enemy& enemy)
{
	m_weponState = StateStorage::WeaponState::None;

	m_stateCurrentTime = 0.0f;

	m_counterOk = false;

	m_attackOk = true;

	m_attackNow = false;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	RenderTargetState _rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

}

void EnemyAttackState::OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
	const Vector3 playerPosition)
{
	m_stateCurrentTime = 0.0f;

	m_counterOk = false;

	m_attackOk = false;

	switch (m_weponState)
	{
	case StateStorage::WeaponState::Spear:
	{
		m_attackNow = false;

		Vector3 _Position = enemy.GetPosition();
		Vector3 _playerPosition = playerPosition;

		//武器を自身の隣にだす用のベクトルを作成
		m_weaponTargetVector = _Position - _playerPosition;
		m_weaponTargetVector.y = 0.0f;
		m_weaponTargetVector.Normalize();

		m_weaponPosition = (ItagakiMath::Cross(Vector3::Up, m_weaponTargetVector) * s_weponDistance) + _Position;

		//武器に進ませるためのベクトルを作成
		m_weaponTargetVector = m_weaponPosition - _playerPosition;
		m_weaponTargetVector.y = 0.0f;
		m_weaponTargetVector.Normalize();

		m_weaponPosition.y += s_weponUpDistance;

		enemy.GetSpear().SetPosition(m_weaponPosition);
		enemy.GetSpear().SetTargetPosition(_playerPosition);
	}
	break;
	case StateStorage::WeaponState::Axe:
	{
		enemy.GetAxe().EnemyHandFollowUpdate(enemy.GetRightHandMatrix(), enemy.GetWorldMatrix());
	}
	break;
	default:
		break;
	}
}

void EnemyAttackState::OnUpdate(Enemy& enemy,
	const SimpleMath::Vector3 playerPosition)
{
	//ステート内時間を更新
	m_stateCurrentTime += DXTK->Time.deltaTime;

	//counter受付時間内の場合処理
	if (m_stateCurrentTime >= s_counterStartTime && m_stateCurrentTime <= s_counterEndTime)
	{
		m_counterOk = true;
	}
	else
	{
		m_counterOk = false;
	}

	switch (m_weponState)
	{
	case StateStorage::WeaponState::Spear:
	{
		if (m_stateCurrentTime >= s_weponLaunchTime && !m_attackNow)
		{
			m_attackNow = true;

			//誘導が適格すぎて避けられないので封印
			//m_weaponTargetVector = playerPosition - m_weaponPosition;
			//m_weaponTargetVector.y = 0.0f;
			//m_weaponTargetVector.Normalize();
			//enemy.GetSpear().SetTargetPosition(playerPosition);
		}

		if (m_attackNow)
		{
			m_weaponPosition = m_weaponPosition + -m_weaponTargetVector * s_spearLaunchSpeed;

			enemy.GetSpear().SetPosition(m_weaponPosition);
		}

		if (m_stateCurrentTime >= s_shotAnimationTime)
		{
			enemy.ChangeState(StateStorage::EnemyState::Stay);
		}
	}
	break;
	case StateStorage::WeaponState::Axe:
	{
		enemy.GetAxe().EnemyHandFollowUpdate(enemy.GetRightHandMatrix(), enemy.GetWorldMatrix());

		if (m_stateCurrentTime >= s_shotAnimationTime)
		{
			enemy.ChangeState(StateStorage::EnemyState::Stay);
		}
	}
	break;
	default:
		break;
	}



}

void EnemyAttackState::OnExit(Enemy& enemy, EnemyStateOrigin& nextState)
{
	m_counterOk = false;

	m_attackOk = true;

	m_attackNow = false;

	enemy.GetSpear().SetPosition(s_screenOffPosition);
}

/// <summary>
/// counterされたとき呼び出される関数
/// </summary>
void EnemyAttackState::OnCounter(Enemy& enemy)
{
	if (m_counterOk)
	{
		enemy.ChangeState(StateStorage::EnemyState::Counter);
	}
}

/// <summary>
/// コライダーが当たった時に呼び出される関数
/// </summary>
void EnemyAttackState::OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider)
{
	if (enemy.GetHitOk())
	{
		enemy.Hit();
	}
}
