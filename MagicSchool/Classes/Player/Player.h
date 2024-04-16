#pragma once

#include <array>

#include"../MainProject/Classes/Descriptors .h"
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Classes/Player/UI/PlayerPowerGauge.h"
#include "Classes/Player/UI/PlayerBulletUI.h"
#include "Classes/Player/Shot/Fire/FireBullet.h"
#include "Classes/Player/Shot/Ice/IceBullet.h"
#include "Classes/Player/Shot/Wind/WindBullet.h"

class Player
{
public:

	void Load(ResourceUploadBatch&,
		const DirectXTK::DescriptorHeap&,
		const Descriptors::Descriptor);

	void Reset();

	void Initialize(const Descriptors::Descriptor);

	void Update();

	void ItemHit();

	void PlayerDraw(SpriteBatch*);

	void UIDraw(SpriteBatch*);

	void BulletDraw(SpriteBatch*);

	SimpleMath::Rectangle GetCollider();

	typedef struct Bullets
	{
		static constexpr unsigned int s_fireCount = 10;

		static constexpr unsigned int s_iceCount = 9;

		static constexpr unsigned int s_windCount = 4;

		FireBullet m_fireBullet[s_fireCount];

		IceBullet m_iceBullet[s_iceCount];

		WindBullet m_windBullet[s_windCount];

	};

	Bullets& GetBulletsStructure();

private:

	void MoveInput();

	void MoveUpdate();

	void Shot();

	void Animation();

	void ShootUpdate();

	void ShootTimeUpdate();

	void ColliderPositionSet();

	void DebugSpritePositionSet();

	DirectX::SimpleMath::Vector2 GetPosition();

	//各属性の弾を打ち出す関数
	void FireShot();
	void IceShot();
	void WindShot();

	//各プレイヤーの中心に至る前進速度を計算し代入する
	void ComeUpSpeedSet(const float);
	//前進速度計算に使う調整変数
	static constexpr unsigned int s_comeUpSpeedSetOffsetPlayer1 = 160;

	static constexpr unsigned int s_comeUpSpeedSetOffsetPlayer2 = 178;

	//プレイヤーの初期位置を設定する
	void DefaultPositionSet(const float);

	enum MoveMode
	{
		Stay, Up, Down
	};
	MoveMode moveMode;

	Bullets m_bullets;

	bool m_isPlayer1;

	PlayerPowerGauge m_powerGauge;

	PlayerBulletUI m_bulletUI;

	DirectXTK::Sprite m_sprite;

	//元画像(1コマ)の大きさ
	static constexpr XMUINT2 s_displaySize = XMUINT2(180, 180);

	const wchar_t* s_player_1FilePass =
		L"Sprite/player/eri_4.png";

	const wchar_t* s_player_1ShootFilePass =
		L"Sprite/player/eri_shoot.png";

	const wchar_t* s_player_2FilePass =
		L"Sprite/player/iris_2.png";

	const wchar_t* s_player_2ShootShootFilePass =
		L"Sprite/player/iris_shoot.png";

	//現在位置
	DirectX::SimpleMath::Vector2 m_position;
	//初期位置の調整変数
	static constexpr unsigned int s_startPositionOffset = 30;

	//弾の発射位置
	DirectX::SimpleMath::Vector2 m_bulletLaunchPosition;

	//発射位置調整変数
	DirectX::SimpleMath::Vector2 m_bulletLaunchPositionOffset;

	//発射位置調整変数のプレイヤー毎の調整用変数
	static constexpr SimpleMath::Vector2 s_bulletLaunchPositionOffsetPlayer1 =
		SimpleMath::Vector2((s_displaySize.x / 6) + 50, (s_displaySize.y / 4));

	static constexpr SimpleMath::Vector2 s_bulletLaunchPositionOffsetPlayer2 =
		SimpleMath::Vector2(-15, (s_displaySize.y / 4));


	//移動速度 アイテム取得で変化するため二つ宣言
	static constexpr float s_defaultSpeed = 1.0f;
	float m_speed;

	//前進速度
	float m_comeUpSpeed;

	//上下速度
	static constexpr float s_verticalitySpeed = 1.0f;

	//画面外に出ないようにする際に使う調整変数
	static constexpr unsigned int s_heightClampOffset = (s_displaySize.y - 15);

	//当たり判定
	SimpleMath::Rectangle m_collider;

	//デバッグ用スプライト
	const wchar_t* s_debugSpriteFilePass =
		L"Sprite/UI/debugSprite.png";
	DirectXTK::Sprite m_debugSprite;
	DirectX::SimpleMath::Vector2 m_debugPosition;

	//限界まで近づくようになるまでの時間
	static constexpr float s_comeUpTime = 60;
	//現在時間
	float m_cuurentTime;
	//お互い近づいていいか知るフラグ
	bool m_comeUpOk;

	//画面に表示される大きさの倍率
	static constexpr float s_drawScale = 0.8f;

	//画面に表示される順番を決めるときに使う調整変数
	static constexpr int s_layerOffset = 100;

	//弾が解放されるアイテムの数
	static constexpr int
		s_iceBulletAuthorizationItemCount = 3;

	static constexpr int
		s_windBulletAuthorizationItemCount = 5;

	//獲得したアイテムの総数
	int m_havingItemCount;

	//アイテム獲得で速度アップする閾値
	static constexpr float s_upSpeed = 1.1f;

	//発射間隔制御
	//火
	static constexpr float s_fireCoolTime = 1.2f;
	float m_fireCurrentTime;
	bool m_fireShotOk;
	//氷
	static constexpr float s_iceCoolTime = 1.2f;
	static constexpr float s_iceBulletDistance = 55.0f;
	float m_iceCurrentTime;
	bool m_iceShotOk;
	//風
	static constexpr float s_windCoolTime = 1.2f;
	float m_windCurrentTime;
	bool m_windShotOk;

	//アニメーション

	//シュートアニメーション

	//待機アニメーション
	static constexpr UINT     s_animationFrame = 120;//アニメーションのそうフレーム数
	static constexpr int	  s_animationHorizontalFrameCount = 12;//画像の横並びの数
	static constexpr float	  s_animationSpeed = 0.02f;
	UINT                      m_animationCount;
	RECT                      m_srcRect;
	float                     m_animationTime;


};

