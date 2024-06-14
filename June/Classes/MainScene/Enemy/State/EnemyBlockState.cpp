#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyBlockState.h"
#include  "Classes/MainScene/Enemy/Enemy.h"

using namespace SimpleMath;

void EnemyBlockState::Initialize(Enemy& enemy)
{
	m_currentTime = 0.0f;
}

void EnemyBlockState::OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
	const Vector3 playerPosition)
{
	m_currentTime = 0.0f;
}

void EnemyBlockState::OnUpdate(Enemy& enemy, const Vector3 playerPosition)
{
	m_currentTime += DXTK->Time.deltaTime;


	if (m_currentTime >= s_blockTime)
	{
		enemy.ChangeState(StateStorage::EnemyState::Stay);
	}
}

void EnemyBlockState::OnExit(Enemy& enemy, EnemyStateOrigin& nextState)
{
	m_currentTime = 0.0f;
}

void EnemyBlockState::OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider)
{
	if (!enemy.GetHitOk())
	{
		return;
	}

	//ïÄÇÕñhå‰ñ≥éãÇµÇƒìñÇΩÇÈ
	if (collider._hitWeaponState == StateStorage::WeaponState::Axe)
	{
		enemy.Hit();
	}

	//spearÇ™ê≥ñ 45ìxÇ…Ç†ÇΩÇ¡ÇΩÇÁñ≥éã
	Vector3 _toSpearVector = collider._hitPosition - enemy.GetPosition();
	_toSpearVector.y = 0.0f;
	_toSpearVector.Normalize();
	Vector3 _forwardVector = enemy.GetWorldMatrix().Forward();
	float _dot = ItagakiMath::Dot(_forwardVector, _toSpearVector);
	if (_dot <= s_blockAngleBack && _dot >= s_blockAngleFront)
	{
		return;
	}

	enemy.Hit();

}