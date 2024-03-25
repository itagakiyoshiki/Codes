#pragma once

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"


class PlayerBulletUI
{
public:
	void Load(ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& m_resourceDescriptors,
		const Descriptors::Descriptor);

	void Reset();

	void Initialize();

	void Update();

	void ItemHit();

	void Draw(SpriteBatch*);

	float GetDrawScale();

	DirectX::SimpleMath::Vector2 GetPosition(Descriptors::Descriptor desc);

	DirectX::SimpleMath::Vector2 GetPosition(int _index);

	//m_bgSprite.handle��Ԃ��֐�
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle(int _index);

	//m_bgSprite.size��Ԃ��֐�
	DirectX::XMUINT2 GetSize(int _index);

	//static constexpr int playerBulletUISpriteCount = 5;
private:

	//�摜�t�@�C���̃p�X
	const wchar_t* s_fireSpriteFilePass =
		L"Sprite/UI/fire.tamasentakiu.UI.V3.png";

	const wchar_t* s_iceSpriteFilePass =
		L"Sprite/UI/ice.tamasentakiu.UI.V3.png";

	const wchar_t* s_windSpriteFilePass =
		L"Sprite/UI/wind.tamasentakiu.UI.V4.png";

	const wchar_t* s_unknownSpriteFilePass =
		L"Sprite/UI/nazo.tamasentakiu.UI.png";

	bool m_isPlayer1;

	Descriptors m_descriptors;

	DirectXTK::Sprite m_fireSprite;
	DirectXTK::Sprite m_iceSprite;
	DirectXTK::Sprite m_windSprite;
	DirectXTK::Sprite m_hiddenSprite_1;
	DirectXTK::Sprite m_hiddenSprite_2;

	//���摜(1�R�})�̑傫��
	static constexpr XMUINT2 s_displaySize =
		XMUINT2(213, 100);

	static constexpr DirectX::SimpleMath::Vector2 s_offPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);


	DirectX::SimpleMath::Vector2 m_firePosition;
	DirectX::SimpleMath::Vector2 m_icePosition;
	DirectX::SimpleMath::Vector2 m_windPosition;
	DirectX::SimpleMath::Vector2 m_hidden_1Position;
	DirectX::SimpleMath::Vector2 m_hidden_2Position;


	//��ʂɕ\������鏇�Ԃ����߂�Ƃ��Ɏg�������ϐ�
	static constexpr int s_layerOffset = 100;

	//��ʂɕ\�������傫���̔{��
	static constexpr float s_drawScale = 1.0f;

	//�e����������A�C�e���̐�
	static constexpr int
		s_iceBulletAuthorizationItemCount = 3;

	static constexpr int
		s_windBulletAuthorizationItemCount = 5;

	//�l�������A�C�e���̑���
	int m_havingItemCount;

};

