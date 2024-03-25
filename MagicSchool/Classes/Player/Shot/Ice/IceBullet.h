#pragma once

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"


class IceBullet
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
		L"Sprite/Effects/ice.png";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/ice_hit.wav";

	//画面に表示される順番を決めるときに使う調整変数
	static constexpr int s_layerOffset = 100;

	//プレイヤー１かどうか見るフラグ
	bool m_isPlayer1;

	//発射できるか否か判別フラグ
	bool m_ShootingOk;

	//元画像
	DirectXTK::Sprite m_sprite;

	//元画像(1コマ)の大きさ
	static constexpr XMUINT2 s_displaySize =
		XMUINT2(64.0f, 64.0f);

	//現在位置
	DirectX::SimpleMath::Vector2 m_position;

	DirectXTK::Sound m_hitSe;

	//当たり判定
	SimpleMath::Rectangle m_collider;

	static constexpr DirectX::SimpleMath::Vector2 s_offPosition =
		DirectX::SimpleMath::Vector2(-100.0f, -100.0f);

	//移動速度
	static constexpr float s_speed = 300.0f;

	//画面に表示される大きさの倍率
	static constexpr float s_drawScale = 1.0f;
};

