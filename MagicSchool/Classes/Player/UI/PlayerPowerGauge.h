#pragma once

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "../MainProject/Classes/Descriptors .h"


class PlayerPowerGauge
{
public:
	void Load(ResourceUploadBatch& resourceUpload,
		const DirectXTK::DescriptorHeap& m_resourceDescriptors,
		const Descriptors::Descriptor);

	void Reset();

	void Initialize();

	void Update();

	void Draw(SpriteBatch*);

	float GetGaugePosition_Y();

	static constexpr int playerBulletUISpriteCount = 5;
private:

	void  FrameBackSpriteDraw(SpriteBatch*);

	void  GaugeSpriteDraw(SpriteBatch*);

	void  FrameSpriteDraw(SpriteBatch*);

	const wchar_t* s_gauge1SpriteFilePass =
		L"Sprite/UI/ge-zi.png";

	const wchar_t* s_gauge2SpriteFilePass =
		L"Sprite/UI/ge-zi.2.png";

	const wchar_t* s_gaugeFrameSpriteFilePass =
		L"Sprite/UI/ge-zi.waku.png";

	const wchar_t* s_lineScaleSpriteFilePass =
		L"Sprite/UI/hidden_sukima (1).png";

	static constexpr float s_playerGaugePositionOffset = 103;

	//画面に表示される順番を決めるときに使う調整変数
	static constexpr int s_layerOffset = 100;

	bool m_isPlayer1;

	Descriptors m_descriptors;

	DirectXTK::Sprite m_gaugeSprite;
	DirectXTK::Sprite m_frameSprite;
	DirectXTK::Sprite m_frameBackSprite;

	//元画像(1コマ)の大きさ
	static constexpr XMUINT2 s_gaugeSize = XMUINT2(639, 65);
	static constexpr XMUINT2 s_frameSize = XMUINT2(639, 65);
	static constexpr XMUINT2 s_frameBackSize = XMUINT2(639, 65);

	DirectX::SimpleMath::Vector2 m_gaugePosition;
	DirectX::SimpleMath::Vector2 m_framePosition;
	DirectX::SimpleMath::Vector2 m_frameBackPosition;

	//画面に表示される大きさの倍率
	static constexpr float s_gaugeScale = 1.0f;
	static constexpr float s_frameScale = 1.0f;
	static constexpr float s_frameBackScale = 1.0f;




};

