#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Descriptors .h"
#include "../MainProject/Scenes/Scene.h"

class RetryButton
{
public:
	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&);

	void Initialize();

	void AnimationStart();

	void AnimationUpdate();

	bool AnimationEnd();

	void Reset();

	void Draw(SpriteBatch*);

private:

	static constexpr SimpleMath::Vector2 s_defaultPosition =
		SimpleMath::Vector2(0.0f, 0.0f);

	const wchar_t* s_spriteFilePass =
		L"Sprite/GameOver/retry.png";

	//‰æ–Ê‚É•\¦‚³‚ê‚é‡”Ô‚ğŒˆ‚ß‚é‚Æ‚«‚Ég‚¤’²®•Ï”
	static constexpr int s_layerOffset = 100;

	DirectXTK::Sprite m_sprite;

	bool m_animationStart;
};

