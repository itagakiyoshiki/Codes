#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "Classes/ColliderInformation.h"

class Enemy;

class EnemyStateOrigin
{
public:

	virtual void Initialize(Enemy& enemy);

	virtual void OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
		const SimpleMath::Vector3 playerPosition);

	virtual void OnUpdate(Enemy& enemy, const SimpleMath::Vector3 playerPosition);

	virtual void OnExit(Enemy& enemy, EnemyStateOrigin& nextState);

	virtual void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

private:


};

