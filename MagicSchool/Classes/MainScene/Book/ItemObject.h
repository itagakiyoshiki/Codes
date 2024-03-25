#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"


class ItemObject
{
public:
	void Load(ResourceUploadBatch&,
		DirectXTK::DescriptorHeap&);

	void Initialize();

	void Update();

	void Pop(DirectX::SimpleMath::Vector2, bool);

	void Hit();

	void Reset();

	void Draw(SpriteBatch*);

	SimpleMath::Rectangle GetCollider();

private:
	const wchar_t* s_spriteFilePass =
		L"Sprite/Battle/Book/book.png";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/item_get.wav";

	DirectXTK::Sound m_hitSe;

	bool m_isPlayer1;

	//���˂ł��邩�ۂ����ʃt���O
	bool m_ShootingOk;

	//���摜
	DirectXTK::Sprite m_sprite;

	//���摜(1�R�})�̑傫��
	static constexpr XMUINT2 s_displaySize =
		XMUINT2(100.0f, 100.0f);

	//���݈ʒu
	DirectX::SimpleMath::Vector2 m_position;

	//�����蔻��
	SimpleMath::Rectangle m_collider;

	static constexpr DirectX::SimpleMath::Vector2 s_offPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);

	//�ړ����x
	static constexpr float s_speed = 150.0f;

	//��ʂɕ\�������傫���̔{��
	static constexpr float s_drawScale = 1.0f;
};

