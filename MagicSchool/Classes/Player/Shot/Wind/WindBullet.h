#pragma once

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"


class WindBullet
{
public:
	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&,
		const Descriptors::Descriptor);

	void Initialize();

	void Update();

	void Shoot(const SimpleMath::Vector2);

	void Hit();

	bool GetShootingOK();

	void Reset();

	void Draw(SpriteBatch*);

	SimpleMath::Rectangle GetCollider();

private:

	void ColliderPositionSet();

	const wchar_t* s_spriteFilePass =
		L"Sprite/Effects/wind.png";

	const wchar_t* s_spriteReverseFilePass =
		L"Sprite/Effects/wind_reverse.png";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/wind_hit.wav";

	//プレイヤー１かどうか見るフラグ
	bool m_isPlayer1;

	//発射できるか否か判別フラグ
	bool m_ShootingOk;

	//元画像
	DirectXTK::Sprite m_sprite;

	//元画像(1コマ)の大きさ
	static constexpr XMUINT2 s_displaySize = XMUINT2(64.0f, 64.0f);

	//現在位置
	DirectX::SimpleMath::Vector2 m_position;

	//発射された原点
	DirectX::SimpleMath::Vector2 m_launchPosition;

	//当たり判定
	SimpleMath::Rectangle m_collider;

	DirectXTK::Sound m_hitSe;

	static constexpr DirectX::SimpleMath::Vector2 s_offPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);

	//画面に表示される順番を決めるときに使う調整変数
	static constexpr int s_layerOffset = 100;

	//移動速度
	static constexpr float s_speed = 300.0f;

	//画面に表示される大きさの倍率
	static constexpr float s_drawScale = 1.0f;

	//正弦波の波の範囲
	static constexpr float s_waveRange = 100;

	//正弦波の範囲を遷移する周期の速度
	static constexpr float s_upDownSpeed = 340;

	float m_signCount;
};

