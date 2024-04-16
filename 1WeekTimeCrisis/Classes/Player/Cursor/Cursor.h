#pragma once
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"

class Cursor
{
public:
	void Load(ResourceUploadBatch&,
		std::unique_ptr<DirectX::DescriptorHeap>&);

	void Initialize();

	void Update();

	void Reset();

	void Draw(SpriteBatch*);

private:

	const wchar_t* s_spriteFilePass =
		L"Sprite/‚PWeekCursor.png";

	static constexpr float s_drawScale = 1.0f;

	static constexpr float s_drawRotation = 0.0f;

	static constexpr int s_layerOffset = 100;

	DirectXTK::Sprite m_sprite;

	DirectX::SimpleMath::Vector2 m_position;

	SimpleMath::Vector2 m_oldPosition;

};

