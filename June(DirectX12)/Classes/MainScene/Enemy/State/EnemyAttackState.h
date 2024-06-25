#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "EnemyStateOrigin.h"
#include "Classes/ItagakiMath.h"
#include "Classes/ColliderInformation.h"

class Enemy;

class EnemyAttackState :public EnemyStateOrigin
{
public:

	void Initialize(Enemy& enemy);

	void OnEnter(Enemy& enemy, EnemyStateOrigin& beforState,
		const SimpleMath::Vector3 playerPosition);

	void OnUpdate(Enemy& enemy,
		const SimpleMath::Vector3 playerPosition);

	void OnExit(Enemy& enemy, EnemyStateOrigin& nextState);

	void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

	void OnCounter(Enemy&);

	void SetWeponState(StateStorage::WeaponState setState)
	{
		m_weponState = setState;
	}

	bool GetAttackNow()
	{
		return m_attackNow;
	}

	bool GetCounterOk()
	{
		return m_counterOk;
	}

private:

	//アニメーションの総時間
	static constexpr float s_shotAnimationTime = 1.2f;

	//武器と自分をどれだけ離すか
	static constexpr float s_weponDistance = 100.0f;
	//武器と地面をどれだけ離すか
	static constexpr float s_weponUpDistance = 100.0f;
	//武器がいつ発射されるか
	static constexpr float s_weponLaunchTime = 0.7f;
	//スピアの発射速度
	static constexpr float s_spearLaunchSpeed = 40.0f;

	//counterの受付時間と終了時間
	static constexpr float s_counterStartTime = 0.2f;
	static constexpr float s_counterEndTime = 0.8f;

	//コライダーの大きさ
	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(10.0f, 60.0f, 10.0f);

	//コライダーが不活性の時の位置
	static constexpr SimpleMath::Vector3 s_screenOffPosition =
		SimpleMath::Vector3(1000, 1000, 1000);

	//攻撃が当たった時のSEのファイルパス
	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	//連続で攻撃に当たらない用にするための変数
	static constexpr float s_hitCoolTIme = 1.0f;

	//攻撃のクールタイム
	static constexpr float s_attackCoolTime = 1.5f;

	//SE
	DirectXTK::Sound m_hitSe;

	SimpleMath::Vector3 m_weaponPosition;
	SimpleMath::Vector3 m_weaponTargetVector;

	StateStorage::WeaponState m_weponState;

	float s_hitCoolCurrentTIme;

	float m_attackCoolCurrentTime;

	float m_stateCurrentTime;

	bool m_counterOk;

	bool m_attackOk;

	bool m_attackNow;

};

