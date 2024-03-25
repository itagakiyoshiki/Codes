#pragma once

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"


class WindBullet
{
public:
	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&,
		const Descriptors::Descriptor);

	void Initialize();

	void Update();

	void Shoot(const SimpleMath::Vector2);

	void Hit();

	bool GetShootingOK();

	void Reset();

	void Draw(SpriteBatch*);

	SimpleMath::Rectangle GetCollider();

private:

	void ColliderPositionSet();

	const wchar_t* s_spriteFilePass =
		L"Sprite/Effects/wind.png";

	const wchar_t* s_spriteReverseFilePass =
		L"Sprite/Effects/wind_reverse.png";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/wind_hit.wav";

	//�v���C���[�P���ǂ�������t���O
	bool m_isPlayer1;

	//���˂ł��邩�ۂ����ʃt���O
	bool m_ShootingOk;

	//���摜
	DirectXTK::Sprite m_sprite;

	//���摜(1�R�})�̑傫��
	static constexpr XMUINT2 s_displaySize = XMUINT2(64.0f, 64.0f);

	//���݈ʒu
	DirectX::SimpleMath::Vector2 m_position;

	//���˂��ꂽ���_
	DirectX::SimpleMath::Vector2 m_launchPosition;

	//�����蔻��
	SimpleMath::Rectangle m_collider;

	DirectXTK::Sound m_hitSe;

	static constexpr DirectX::SimpleMath::Vector2 s_offPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);

	//��ʂɕ\������鏇�Ԃ����߂�Ƃ��Ɏg�������ϐ�
	static constexpr int s_layerOffset = 100;

	//�ړ����x
	static constexpr float s_speed = 300.0f;

	//��ʂɕ\�������傫���̔{��
	static constexpr float s_drawScale = 1.0f;

	//�����g�̔g�͈̔�
	static constexpr float s_waveRange = 100;

	//�����g�͈̔͂�J�ڂ�������̑��x
	static constexpr float s_upDownSpeed = 340;

	float m_signCount;
};

