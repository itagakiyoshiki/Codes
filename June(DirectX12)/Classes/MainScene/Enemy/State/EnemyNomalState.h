#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "EnemyStateOrigin.h"
#include "Classes/ColliderInformation.h"

class Enemy;

class EnemyNomalState :public EnemyStateOrigin
{
public:

	void Initialize(Enemy& enemy);

	void OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
		const SimpleMath::Vector3 playerPosition);

	void OnUpdate(Enemy& enemy,
		const SimpleMath::Vector3 playerPosition);

	void OnExit(Enemy& enemy, EnemyStateOrigin& nextState);

	void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

	void CounterOn()
	{
		m_counterOn = true;
	}

	void SetCounterOn(bool set)
	{
		m_counterCurrentTime = 0.0f;
		m_counterOn = set;
	}

	bool GetCounterOn()
	{
		return m_counterOn;
	}

	SimpleMath::Vector3& GetPlayerPosition()
	{
		return m_playerPosition;
	}

private:

	void OutUpdate(Enemy& enemy);
	void FarUpdate(Enemy& enemy);
	void MidUpdate(Enemy& enemy);
	void NearUpdate(Enemy& enemy);

	void DistanceStateUpdate();

	const enum class DistanceState
	{
		Out, Far, Mid, Near
	};

	const enum class MidActionState
	{
		Left, Right, Count, None
	};

	//移動速度
	static constexpr float s_farSpeed = 500.0f;
	static constexpr float s_midSpeed = 25.0f;
	//距離判別変数
	static constexpr float s_farDistance = 800.0f;
	static constexpr float s_midDistance = 400.0f;
	static constexpr float s_nearDistance = 200.0f;

	//被カウンターモーション時間
	static constexpr float s_counterTime = 1.25f;

	DistanceState m_currentDistanceState;
	DistanceState m_beforeDistanceState;

	MidActionState  m_currentMidActionState;

	SimpleMath::Vector3 m_playerPosition;
	SimpleMath::Vector3 m_myPosition;

	float m_counterCurrentTime;

	bool m_counterOn;

	bool m_stateChangeOn;

};

