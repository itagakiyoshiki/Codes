#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "EnemyStateOrigin.h"
#include "Classes/ColliderInformation.h"
#include "Classes/ItagakiMath.h"

class Enemy;

class EnemyBlockState :public EnemyStateOrigin
{
public:

	virtual void Initialize(Enemy& enemy);

	virtual void OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
		const SimpleMath::Vector3 playerPosition);

	virtual void OnUpdate(Enemy& enemy, const SimpleMath::Vector3 playerPosition);

	virtual void OnExit(Enemy& enemy, EnemyStateOrigin& nextState);

	virtual void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

private:

	static constexpr float s_blockTime = 100.5f;

	static constexpr float s_blockAngleFront = 0.0f;
	static constexpr float s_blockAngleBack = 1.0f;

	float m_currentTime;

};

