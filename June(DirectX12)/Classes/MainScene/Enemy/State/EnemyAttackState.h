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

	//�A�j���[�V�����̑�����
	static constexpr float s_shotAnimationTime = 1.2f;

	//����Ǝ������ǂꂾ��������
	static constexpr float s_weponDistance = 100.0f;
	//����ƒn�ʂ��ǂꂾ��������
	static constexpr float s_weponUpDistance = 100.0f;
	//���킪�����˂���邩
	static constexpr float s_weponLaunchTime = 0.7f;
	//�X�s�A�̔��ˑ��x
	static constexpr float s_spearLaunchSpeed = 40.0f;

	//counter�̎�t���ԂƏI������
	static constexpr float s_counterStartTime = 0.2f;
	static constexpr float s_counterEndTime = 0.8f;

	//�R���C�_�[�̑傫��
	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(10.0f, 60.0f, 10.0f);

	//�R���C�_�[���s�����̎��̈ʒu
	static constexpr SimpleMath::Vector3 s_screenOffPosition =
		SimpleMath::Vector3(1000, 1000, 1000);

	//�U����������������SE�̃t�@�C���p�X
	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	//�A���ōU���ɓ�����Ȃ��p�ɂ��邽�߂̕ϐ�
	static constexpr float s_hitCoolTIme = 1.0f;

	//�U���̃N�[���^�C��
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

