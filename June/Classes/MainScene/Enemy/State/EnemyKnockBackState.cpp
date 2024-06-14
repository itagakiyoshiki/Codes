#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyKnockBackState.h"
#include  "Classes/MainScene/Enemy/Enemy.h"

using namespace SimpleMath;

void EnemyKnockBackState::Initialize(Enemy& enemy)
{
	m_animationCurrentTime = 0.0f;

	m_knockBackVector = Vector3::Zero;
}

void EnemyKnockBackState::OnEnter(Enemy& enemy, EnemyStateOrigin& beforState, const SimpleMath::Vector3 playerPosition)
{
	m_animationCurrentTime = 0.0f;

	m_knockBackVector = enemy.GetPosition() - playerPosition;
	m_knockBackVector.y = 0.0f;
	m_knockBackVector.Normalize();
}

void EnemyKnockBackState::OnUpdate(Enemy& enemy, const SimpleMath::Vector3 playerPosition)
{
	m_animationCurrentTime += DXTK->Time.deltaTime;

	//ノックバック時間いっぱい吹き飛ぶ
	if (m_animationCurrentTime <= s_knockBackEndTime)
	{
		enemy.SetPosition(enemy.GetPosition() + m_knockBackVector * s_knockBackPower * DXTK->Time.deltaTime);
	}

	if (m_animationCurrentTime >= s_animationTime)
	{
		m_animationCurrentTime = 0.0f;
		enemy.ChangeState(StateStorage::EnemyState::Stay);
	}
}

void EnemyKnockBackState::OnExit(Enemy& enemy, EnemyStateOrigin& nextState)
{
	m_animationCurrentTime = 0.0f;
}

void EnemyKnockBackState::OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider)
{

}