#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "EnemyStateOrigin.h"

class Enemy;

class EnemyKnockBackState :public EnemyStateOrigin
{
public:

	virtual void Initialize(Enemy& enemy);

	virtual void OnEnter(Enemy& enemy,
		EnemyStateOrigin& beforState, const SimpleMath::Vector3 playerPosition);

	virtual void OnUpdate(Enemy& enemy, const SimpleMath::Vector3 playerPosition);

	virtual void OnExit(Enemy& enemy, EnemyStateOrigin& nextState);

	virtual void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

private:
	//アニメーションの総時間
	static constexpr float s_animationTime = 1.7f;

	//ノックバックの終わる時間
	static constexpr float s_knockBackEndTime = 1.0f;
	//ノックバックの力
	static constexpr float s_knockBackPower = 500.0f;

	SimpleMath::Vector3 m_knockBackVector;

	float m_animationCurrentTime;

	bool m_knockBackOk;

};

