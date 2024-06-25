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

	//�A�j���[�V�����̑�����
	static constexpr float s_animationTime = 1.5f;

	//�U����������������SE�̃t�@�C���p�X
	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	static constexpr float s_comboInputTime = 0.5f;

	//����ƃv���C���[���ǂꂾ��������
	static constexpr float s_weponDistance = 100.0f;
	//����ƒn�ʂ��ǂꂾ��������
	static constexpr float s_weponUpDistance = 100.0f;
	//���킪�����˂���邩
	static constexpr float s_weponLaunchTime = 0.6f;
	//�X�s�A�̔��ˑ��x
	static constexpr float s_spearLaunchSpeed = 100.0f;
	//���� ����/s �̐��l
	static constexpr float s_axeLaunchSpeed = 360.0f;
	//���̉�]�l�̒����l
	static constexpr float s_axeDefaultRotationOffset = 90.0f * Mathf::Deg2Rad;

	static constexpr float s_defaultScale = 1.0f;
	static constexpr float s_backRotation = 180.0f * Mathf::Deg2Rad;

	//�X�s�A�p�̈ʒu
	SimpleMath::Vector3 m_weaponPosition;
	//�X�s�A�����ł��x�N�g��
	SimpleMath::Vector3 m_spearTargetVector;

	StateStorage::WeaponState m_weponState;
	StateStorage::WeaponState m_nextWeponState;

	//SE
	DirectXTK::Sound m_hitSe;

	//���̉~���p�ϐ�
	float m_axeTheta;
	float m_axeEndTheta;

	float m_hitCoolCurrentTIme;

	float m_attackCoolCurrentTime;

	float m_animationCurrentTime;

	bool m_attackNow;

	bool m_weponLaunchOn;

	bool m_spearOn;

};

