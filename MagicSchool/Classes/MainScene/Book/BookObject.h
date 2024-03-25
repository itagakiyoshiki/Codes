#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"
#include "ItemObject.h"


class BookObject
{
public:
	void Load(ResourceUploadBatch&,
		DirectXTK::DescriptorHeap&);

	void Initialize();

	void Update();

	void Hit();

	void RePop();

	void Reset();

	void Draw(SpriteBatch*);

	SimpleMath::Vector2 GetPosition();

	SimpleMath::Rectangle GetCollider();

private:

	enum MoveState
	{
		Up, Down
	};

	MoveState m_moveState;

	static constexpr SimpleMath::Vector2 s_screenOffPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);

	const wchar_t* s_spriteFilePass =
		L"Sprite/Battle/Book/book.png";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/fire_hit.wav";

	//���摜(1�R�})�̑傫��
	static constexpr XMUINT2 s_displaySize =
		XMUINT2(100.0f, 100.0f);

	//�ړ����x
	static constexpr float s_speed = 150.0f;

	//��ʂɕ\�������傫���̔{��
	static constexpr float s_drawScale = 1.0f;

	static constexpr float s_centerLinePositionOffset = 52;

	//���摜
	DirectXTK::Sprite m_sprite;

	//���݈ʒu
	DirectX::SimpleMath::Vector2 m_position;

	//�����蔻��
	SimpleMath::Rectangle m_collider;

	ItemObject m_itemObject;

	float m_centerLinePosition;

	static constexpr float s_upperLimit = 0.0f - 152.0f;

	static constexpr float s_lowerLimitOffset = 52.0f;

	static constexpr float s_rePopTime = 3.0f;
	float m_rePopCurrentTime;

	float m_lowerLimit;

	bool m_bulletHit;

	bool m_rePopOk;

};

