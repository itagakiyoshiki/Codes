#pragma once
#include "Scenes/Scene.h"
#include "StateOrigin.h"
#include "Classes/ItagakiMath.h"

class Player;

class AttackState : public StateOrigin
{
public:
	void Initialize(RenderTargetState);

	void OnEnter(Player&, StateOrigin&);

	void OnUpdate(Player&, const DirectXTK::Camera& camera);

	void OnExit(Player&, StateOrigin&);

	void OnCollisionEnter();

	void SetWeponState(StateStorage::WeaponState setState)
	{
		m_weponState = setState;
	}

	const bool GetAttackNow()
	{
		return m_attackNow;
	}

	const bool& GetSpearOn()
	{
		return m_spearOn;
	}

private:

	SimpleMath::Matrix ModelAttackLookAt(SimpleMath::Vector3 target, SimpleMath::Vector3 position);

	//アニメーションの総時間
	static constexpr float s_animationTime = 1.5f;

	//攻撃が当たった時のSEのファイルパス
	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	static constexpr float s_comboInputTime = 0.5f;

	//武器とプレイヤーをどれだけ離すか
	static constexpr float s_weponDistance = 100.0f;
	//武器と地面をどれだけ離すか
	static constexpr float s_weponUpDistance = 100.0f;
	//武器がいつ発射されるか
	static constexpr float s_weponLaunchTime = 0.6f;
	//スピアの発射速度
	static constexpr float s_spearLaunchSpeed = 100.0f;
	//斧の 半周/s の数値
	static constexpr float s_axeLaunchSpeed = 360.0f;
	//斧の回転値の調整値
	static constexpr float s_axeDefaultRotationOffset = 90.0f * Mathf::Deg2Rad;

	static constexpr float s_defaultScale = 1.0f;
	static constexpr float s_backRotation = 180.0f * Mathf::Deg2Rad;

	//スピア用の位置
	SimpleMath::Vector3 m_weaponPosition;
	//スピアが飛んでくベクトル
	SimpleMath::Vector3 m_spearTargetVector;

	StateStorage::WeaponState m_weponState;
	StateStorage::WeaponState m_nextWeponState;

	//SE
	DirectXTK::Sound m_hitSe;

	//斧の円周用変数
	float m_axeTheta;
	float m_axeEndTheta;

	float m_hitCoolCurrentTIme;

	float m_attackCoolCurrentTime;

	float m_animationCurrentTime;

	bool m_attackNow;

	bool m_weponLaunchOn;

	bool m_spearOn;

};

