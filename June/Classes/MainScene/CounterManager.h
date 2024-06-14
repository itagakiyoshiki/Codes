#pragma once
#include "Scenes/Scene.h"
#include "Classes/MainScene/Enemy/Enemy.h"
#include "Classes/DescriptorStorage.h"

class Enemy;

class CounterManager
{
public:

	void Load(ResourceUploadBatch&,
		std::unique_ptr<DirectX::DescriptorHeap>&);

	void Initialize();

	void Update(Enemy& enemy, DirectXTK::Camera camera);

	void OnDeviceLost();

	void Draw(SpriteBatch*);

	void OnCounter(const SimpleMath::Vector3& playerPosition);

private:

	SimpleMath::Vector2 WorldToScreenPos(SimpleMath::Vector3 worldPos, DirectXTK::Camera camera);

	const wchar_t* s_spriteFilePass =
		L"Sprite/ÇPWeekCursor.png";

	static constexpr float s_drawScale = 1.0f;

	static constexpr float s_drawRotation = 0.0f;

	static constexpr float s_spriteOffsetX = 2.0f;
	static constexpr float s_spriteOffsetY = 1.0f;

	//counterê¨óßÇ∑ÇÈãóó£
	static constexpr float s_counterDistance = 500.0f;

	SimpleMath::Vector3 m_playerPosition;

	SimpleMath::Vector2 m_spriteOldPosition;

	DirectX::SimpleMath::Vector2 m_spritePosition;

	DirectXTK::Sprite m_sprite;

	bool m_counterOn;

	bool m_spriteOn;

};

