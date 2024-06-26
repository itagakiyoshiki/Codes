#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"


class GameEndButton
{
public:

	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&);

	void Reset();

	void Draw(SpriteBatch*);

private:
	DirectXTK::Sprite m_sprite;

	static constexpr SimpleMath::Vector2 s_defaultPosition =
		SimpleMath::Vector2(0.0f, 0.0f);

	const wchar_t* s_spriteFilePass =
		L"Sprite/Battle/battle_bg.png";

	//画面に表示される順番を決めるときに使う調整変数
	static constexpr int s_layerOffset = 100;

};

