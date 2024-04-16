#pragma once

#include <array>

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Player/UI/PlayerPowerGauge.h"
#include "Classes/Player/UI/PlayerBulletUI.h"
#include "Classes/Player/Shot/Fire/FireBullet.h"
#include "Classes/Player/Shot/Ice/IceBullet.h"
#include "Classes/Player/Shot/Wind/WindBullet.h"

class Player
{
public:

	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&,
		const Descriptors::Descriptor);

	void Reset();

	void Initialize(const Descriptors::Descriptor);

	void Update();

	void ItemHit();

	void PlayerDraw(SpriteBatch*);

	void UIDraw(SpriteBatch*);

	void BulletDraw(SpriteBatch*);

	SimpleMath::Rectangle GetCollider();

	typedef struct Bullets
	{
		static constexpr unsigned int s_fireCount = 10;

		static constexpr unsigned int s_iceCount = 9;

		static constexpr unsigned int s_windCount = 4;

		FireBullet m_fireBullet[s_fireCount];

		IceBullet m_iceBullet[s_iceCount];

		WindBullet m_windBullet[s_windCount];

	};

	Bullets& GetBulletsStructure();

private:

	void MoveInput();

	void MoveUpdate();

	void Shot();

	void Animation();

	void ShootUpdate();

	void ShootTimeUpdate();

	void ColliderPositionSet();

	void DebugSpritePositionSet();

	DirectX::SimpleMath::Vector2 GetPosition();

	//�e�����̒e��ł��o���֐�
	void FireShot();
	void IceShot();
	void WindShot();

	//�e�v���C���[�̒��S�Ɏ���O�i���x���v�Z���������
	void ComeUpSpeedSet(const float);
	//�O�i���x�v�Z�Ɏg�������ϐ�
	static constexpr unsigned int s_comeUpSpeedSetOffsetPlayer1 = 160;

	static constexpr unsigned int s_comeUpSpeedSetOffsetPlayer2 = 178;

	//�v���C���[�̏����ʒu��ݒ肷��
	void DefaultPositionSet(const float);

	enum MoveMode
	{
		Stay, Up, Down
	};
	MoveMode moveMode;

	Bullets m_bullets;

	bool m_isPlayer1;

	PlayerPowerGauge m_powerGauge;

	PlayerBulletUI m_bulletUI;

	DirectXTK::Sprite m_sprite;

	//���摜(1�R�})�̑傫��
	static constexpr XMUINT2 s_displaySize = XMUINT2(180, 180);

	const wchar_t* s_player_1FilePass =
		L"Sprite/player/eri_4.png";

	const wchar_t* s_player_1ShootFilePass =
		L"Sprite/player/eri_shoot.png";

	const wchar_t* s_player_2FilePass =
		L"Sprite/player/iris_2.png";

	const wchar_t* s_player_2ShootShootFilePass =
		L"Sprite/player/iris_shoot.png";

	//���݈ʒu
	DirectX::SimpleMath::Vector2 m_position;
	//�����ʒu�̒����ϐ�
	static constexpr unsigned int s_startPositionOffset = 30;

	//�e�̔��ˈʒu
	DirectX::SimpleMath::Vector2 m_bulletLaunchPosition;

	//���ˈʒu�����ϐ�
	DirectX::SimpleMath::Vector2 m_bulletLaunchPositionOffset;

	//���ˈʒu�����ϐ��̃v���C���[���̒����p�ϐ�
	static constexpr SimpleMath::Vector2 s_bulletLaunchPositionOffsetPlayer1 =
		SimpleMath::Vector2((s_displaySize.x / 6) + 50, (s_displaySize.y / 4));

	static constexpr SimpleMath::Vector2 s_bulletLaunchPositionOffsetPlayer2 =
		SimpleMath::Vector2(-15, (s_displaySize.y / 4));


	//�ړ����x �A�C�e���擾�ŕω����邽�ߓ�錾
	static constexpr float s_defaultSpeed = 1.0f;
	float m_speed;

	//�O�i���x
	float m_comeUpSpeed;

	//�㉺���x
	static constexpr float s_verticalitySpeed = 1.0f;

	//��ʊO�ɏo�Ȃ��悤�ɂ���ۂɎg�������ϐ�
	static constexpr unsigned int s_heightClampOffset = (s_displaySize.y - 15);

	//�����蔻��
	SimpleMath::Rectangle m_collider;

	//�f�o�b�O�p�X�v���C�g
	const wchar_t* s_debugSpriteFilePass =
		L"Sprite/UI/debugSprite.png";
	DirectXTK::Sprite m_debugSprite;
	DirectX::SimpleMath::Vector2 m_debugPosition;

	//���E�܂ŋ߂Â��悤�ɂȂ�܂ł̎���
	static constexpr float s_comeUpTime = 60;
	//���ݎ���
	float m_cuurentTime;
	//���݂��߂Â��Ă������m��t���O
	bool m_comeUpOk;

	//��ʂɕ\�������傫���̔{��
	static constexpr float s_drawScale = 0.8f;

	//��ʂɕ\������鏇�Ԃ����߂�Ƃ��Ɏg�������ϐ�
	static constexpr int s_layerOffset = 100;

	//�e����������A�C�e���̐�
	static constexpr int
		s_iceBulletAuthorizationItemCount = 3;

	static constexpr int
		s_windBulletAuthorizationItemCount = 5;

	//�l�������A�C�e���̑���
	int m_havingItemCount;

	//�A�C�e���l���ő��x�A�b�v����臒l
	static constexpr float s_upSpeed = 1.1f;

	//���ˊԊu����
	//��
	static constexpr float s_fireCoolTime = 1.2f;
	float m_fireCurrentTime;
	bool m_fireShotOk;
	//�X
	static constexpr float s_iceCoolTime = 1.2f;
	static constexpr float s_iceBulletDistance = 55.0f;
	float m_iceCurrentTime;
	bool m_iceShotOk;
	//��
	static constexpr float s_windCoolTime = 1.2f;
	float m_windCurrentTime;
	bool m_windShotOk;

	//�A�j���[�V����

	//�V���[�g�A�j���[�V����

	//�ҋ@�A�j���[�V����
	static constexpr UINT     s_animationFrame = 120;//�A�j���[�V�����̂����t���[����
	static constexpr int	  s_animationHorizontalFrameCount = 12;//�摜�̉����т̐�
	static constexpr float	  s_animationSpeed = 0.02f;
	UINT                      m_animationCount;
	RECT                      m_srcRect;
	float                     m_animationTime;


};

