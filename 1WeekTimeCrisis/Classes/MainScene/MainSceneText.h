#pragma once
#include "Scenes/Scene.h"
#include "Classes/Descriptors .h"

class MainSceneText
{
public:
	void CreateText();

	void Load(ResourceUploadBatch&,
		std::unique_ptr<DirectX::DescriptorHeap>&);

	void Initialize();

	void GameEnd(bool);

	void Reset();

	void Draw(SpriteBatch*);


private:
	const wchar_t* s_gameClearFilePass =
		L"Sprite/text_gameclear_j.png";

	const wchar_t* s_gameOverFilePass =
		L"Sprite/text_gameover_j.png";

	const wchar_t* s_operationFilePass =
		L"Sprite/2024-04-05_08h44_39.png";

	static constexpr SimpleMath::Vector2 s_gameClearSpritePosition =
		SimpleMath::Vector2(0, 0);

	static constexpr SimpleMath::Vector2 s_gameOverSpritePosition =
		SimpleMath::Vector2(0, 0);

	static constexpr float s_operationSpriteXOffset = 400;
	static constexpr float s_operationSpriteYOffset = 300;

	static constexpr float s_spriteRotation = 0.0f;

	static constexpr float s_gameClearSpriteDrawScale = 0.25f;

	static constexpr float s_gameOverSpriteDrawScale = 0.25f;

	static constexpr float s_operationDrawScale = 1.0f;

	static constexpr int s_layerOffset = 100;

	DirectXTK::Sprite m_gameClearSprite;

	DirectXTK::Sprite m_gameOverSprite;

	DirectXTK::Sprite m_operationSprite;

	DirectX::SimpleMath::Vector2 m_gameClearPosition;

	DirectX::SimpleMath::Vector2 m_gameOverPosition;

	DirectX::SimpleMath::Vector2 m_operationPosition;

	bool m_gameEnd;

	bool m_gameOver;
};

