#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"

class BackGrond
{
public:
	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&,
		const Descriptors::Descriptor);

	void Reset();

	void Draw(SpriteBatch*);

private:
	DirectXTK::Sprite m_sprite;

	static constexpr SimpleMath::Vector2 s_defaultPosition =
		SimpleMath::Vector2(0.0f, 0.0f);

	//��ʂɕ\������鏇�Ԃ����߂�Ƃ��Ɏg�������ϐ�
	static constexpr int s_layerOffset = 100;

	const wchar_t* s_battleBgFilePass =
		L"Sprite/Battle/battle_bg.png";

	const wchar_t* s_resultBgFilePass =
		L"Sprite/GameOver/rizaruto.png";

	static constexpr float s_drawScale = 1.0f;

};

