#include "Player.h"

void Player::Load(ResourceUploadBatch& esourceUpload,
	const DirectXTK::DescriptorHeap& resourceDescriptors,
	const Descriptors::Descriptor descriptors)
{
	std::wstring fileName = {};

	Descriptors::Descriptor _descript;

	if (descriptors == Descriptors::Descriptor::Player_1)
	{
		fileName = s_player_1FilePass;
		_descript = Descriptors::Descriptor::Debug_1;
		m_isPlayer1 = true;
	}
	else
	{
		fileName = s_player_2FilePass;
		_descript = Descriptors::Descriptor::Debug_2;
		m_isPlayer1 = false;
	}

	//プレイヤー画像読み込み
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		fileName.c_str(),
		esourceUpload,
		resourceDescriptors.get(),
		descriptors);

	//デバッグ用画像の読み込み
	m_debugSprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_debugSpriteFilePass,
		esourceUpload,
		resourceDescriptors.get(),
		_descript);

	//弾の画像の読み込み
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Load(esourceUpload, resourceDescriptors, descriptors);
	}

	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Load(esourceUpload, resourceDescriptors, descriptors);
	}

	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Load(esourceUpload, resourceDescriptors, descriptors);
	}

	//UIの画像の読み込み
	m_bulletUI.Load(esourceUpload, resourceDescriptors, descriptors);
	m_powerGauge.Load(esourceUpload, resourceDescriptors, descriptors);


}

void Player::Initialize(const Descriptors::Descriptor playerIndex)
{
	m_speed = s_defaultSpeed;

	m_havingItemCount = 0;

	m_animationTime = 0.0f;

	m_animationCount = 0.0f;

	m_srcRect = RectWH(0, 0, s_displaySize.x, s_displaySize.y);

	//プレイヤーの初期位置を設定する
	DefaultPositionSet(DXTK->Screen.Width);

	//画面中央に寄っていいか判別フラグ
	m_comeUpOk = true;
	m_cuurentTime = 0.0f;

	//各プレイヤーの中心に至る前進速度を計算し代入する
	ComeUpSpeedSet(DXTK->Screen.Width);

	//動きのモードを待機にして操作されるまでそのままのy軸の位置に居る
	moveMode = Player::MoveMode::Stay;

	//当たり判定設定
	ColliderPositionSet();
	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

	//デバッグ用画像設定
	DebugSpritePositionSet();

	//弾
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Initialize();
	}

	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Initialize();
	}

	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Initialize();
	}

	//それぞれの弾のクールダウン用タイマー初期化
	//弾を打っていいかフラグの初期化
	//火
	m_fireCurrentTime = 0.0f;
	m_fireShotOk = true;
	//氷
	m_iceCurrentTime = 0.0f;
	m_iceShotOk = true;
	//風
	m_windCurrentTime = 0.0f;
	m_windShotOk = true;

	//ゲージのUIの初期化
	m_powerGauge.Initialize();

	//弾のUIの初期化
	m_bulletUI.Initialize();
}

void Player::Update()
{
	MoveInput();

	MoveUpdate();

	Shot();

	Animation();
}

/// <summary>
/// 操作を受け付けて動くモードを切り替える
/// </summary>
void Player::MoveInput()
{
	if (m_isPlayer1)
	{
		if (InputSystem.Keyboard.isPressed.S)
		{
			moveMode = Player::MoveMode::Down;
		}
		else if (InputSystem.Keyboard.isPressed.W)
		{
			moveMode = Player::MoveMode::Up;
		}
	}
	else
	{
		if (InputSystem.Keyboard.isPressed.Down)
		{
			moveMode = Player::MoveMode::Down;
		}
		else if (InputSystem.Keyboard.isPressed.Up)
		{
			moveMode = Player::MoveMode::Up;
		}
	}

}

/// <summary>
/// 自分の動きステートにより動き方を変える
/// </summary>
void Player::MoveUpdate()
{
	DirectX::SimpleMath::Vector2 direction = DirectX::SimpleMath::Vector2::Zero;

	//上下運動
	if (moveMode == Player::MoveMode::Up)
	{
		direction.y += -s_verticalitySpeed * m_speed;
	}
	else if (moveMode == Player::MoveMode::Down)
	{
		direction.y += s_verticalitySpeed * m_speed;
	}

	//中心に寄る運動
	if (m_comeUpOk)
	{
		if (m_isPlayer1)
		{
			direction.x += m_comeUpSpeed * DXTK->Time.deltaTime;
		}
		else
		{
			direction.x += -m_comeUpSpeed * DXTK->Time.deltaTime;
		}
	}

	//中心に寄ってたら寄らない
	if (m_cuurentTime >= s_comeUpTime && m_comeUpOk)
	{
		m_comeUpOk = false;
	}
	else
	{
		m_cuurentTime += DXTK->Time.deltaTime;
	}

	//移動を実行
	m_position += direction;

	//画面外に出ないように位置調整
	m_position.x = std::clamp(
		m_position.x,
		0.0f, float(DXTK->Screen.Width - s_displaySize.x)
	);
	m_position.y = std::clamp(
		m_position.y,
		0.0f, m_powerGauge.GetGaugePosition_Y() - s_heightClampOffset
	);

	//コライダー位置調整
	ColliderPositionSet();

	//デバッグ用画像位置調整
	DebugSpritePositionSet();

	//弾用の発射位置の調整
	m_bulletLaunchPosition = m_position + m_bulletLaunchPositionOffset;
}

void Player::ColliderPositionSet()
{
	m_collider.x = m_position.x;
	m_collider.y = m_position.y;
}

void Player::DebugSpritePositionSet()
{
	m_debugPosition.x = m_collider.x;
	m_debugPosition.y = m_collider.y;
}

void Player::ItemHit()
{
	m_speed *= s_upSpeed;
	m_havingItemCount++;
	m_bulletUI.ItemHit();
}

void Player::Shot()
{
	ShootTimeUpdate();

	if (m_isPlayer1)
	{
		//火
		if (InputSystem.Keyboard.wasPressedThisFrame.Z)
		{
			FireShot();
		}

		//氷
		if (InputSystem.Keyboard.wasPressedThisFrame.X
			&& m_havingItemCount >= s_iceBulletAuthorizationItemCount)
		{
			IceShot();
		}

		//風
		if (InputSystem.Keyboard.wasPressedThisFrame.C
			&& m_havingItemCount >= s_windBulletAuthorizationItemCount)
		{
			WindShot();
		}
	}
	else
	{
		if (InputSystem.Keyboard.wasPressedThisFrame.NumPad1)
		{
			FireShot();
		}

		if (InputSystem.Keyboard.wasPressedThisFrame.NumPad2
			&& m_havingItemCount >= s_iceBulletAuthorizationItemCount)
		{
			IceShot();
		}

		if (InputSystem.Keyboard.wasPressedThisFrame.NumPad3
			&& m_havingItemCount >= s_windBulletAuthorizationItemCount)
		{
			WindShot();
		}
	}

	//位置更新
	ShootUpdate();
}

/// <summary>
/// 撃ってない火を撃つ
/// </summary>
void Player::FireShot()
{
	//火の弾を発射可能になっていたら撃つ
	if (!m_fireShotOk)
	{
		return;
	}

	//火の弾を撃ったので発射不可にして発射可能か判断に流す
	m_fireShotOk = false;
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		if (m_bullets.m_fireBullet[i].GetShootingOK())
		{
			m_bullets.m_fireBullet[i].Shoot(m_bulletLaunchPosition);
			break;
		}
	}
}

/// <summary>
/// 撃ってない氷を撃つ
/// </summary>
void Player::IceShot()
{
	if (!m_iceShotOk)
	{
		return;
	}

	m_iceShotOk = false;
	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		if (!m_bullets.m_iceBullet[i].GetShootingOK())
		{
			continue;
		}

		if (m_isPlayer1)
		{
			SimpleMath::Vector2 _launchPositon = m_bulletLaunchPosition;
			m_bullets.m_iceBullet[i].Shoot(_launchPositon);

			_launchPositon.x -= s_iceBulletDistance;
			m_bullets.m_iceBullet[i + 1].Shoot(_launchPositon);

			_launchPositon.x -= s_iceBulletDistance;
			m_bullets.m_iceBullet[i + 2].Shoot(_launchPositon);
		}
		else
		{
			SimpleMath::Vector2 _launchPositon = m_bulletLaunchPosition;
			m_bullets.m_iceBullet[i].Shoot(_launchPositon);

			_launchPositon.x += s_iceBulletDistance;
			m_bullets.m_iceBullet[i + 1].Shoot(_launchPositon);

			_launchPositon.x += s_iceBulletDistance;
			m_bullets.m_iceBullet[i + 2].Shoot(_launchPositon);
		}

		break;
	}
}

/// <summary>
/// 撃ってない風を撃つ
/// </summary>
void Player::WindShot()
{

	if (!m_windShotOk)
	{
		return;
	}

	m_windShotOk = false;
	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		if (m_bullets.m_windBullet[i].GetShootingOK())
		{
			m_bullets.m_windBullet[i].Shoot(m_bulletLaunchPosition);
			break;
		}
	}
}

/// <summary>
/// 各弾の位置を動かす
/// </summary>
void Player::ShootUpdate()
{
	//火
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Update();
	}

	//氷
	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Update();
	}

	//風
	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Update();
	}
}

/// <summary>
/// 各弾の発射クールタイムを計測
/// </summary>
void Player::ShootTimeUpdate()
{
	//火
	if (!m_fireShotOk)
	{
		m_fireCurrentTime += DXTK->Time.deltaTime;
		if (m_fireCurrentTime >= s_fireCoolTime)
		{
			m_fireCurrentTime = 0.0f;
			m_fireShotOk = true;
		}
	}

	//氷
	if (!m_iceShotOk)
	{
		m_iceCurrentTime += DXTK->Time.deltaTime;
		if (m_iceCurrentTime >= s_iceCoolTime)
		{
			m_iceCurrentTime = 0.0f;
			m_iceShotOk = true;
		}
	}

	//風
	if (!m_windShotOk)
	{
		m_windCurrentTime += DXTK->Time.deltaTime;
		if (m_windCurrentTime >= s_windCoolTime)
		{
			m_windCurrentTime = 0.0f;
			m_windShotOk = true;
		}
	}

}

void Player::Animation()
{

	// 待機アニメーション
	m_animationTime += DXTK->Time.deltaTime;
	if (m_animationTime >= s_animationSpeed)
	{
		m_animationTime -= s_animationSpeed;
		m_animationCount = (m_animationCount + 1) % s_animationFrame;
		m_srcRect.left = (m_animationCount % s_animationHorizontalFrameCount) * s_displaySize.x;
		m_srcRect.top = (m_animationCount / s_animationHorizontalFrameCount) * s_displaySize.y;
		m_srcRect.right = m_srcRect.left + s_displaySize.x;
		m_srcRect.bottom = m_srcRect.top + s_displaySize.y;
	}
}

void Player::PlayerDraw(SpriteBatch* batch)
{
	//プレイヤー画像書き出し
	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, &m_srcRect,
		Colors::White, 0.0f, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Player_1 / s_layerOffset);

	//デバッグ用画像書き出し
	//DirectX::XMVECTORF32 _color = { {{(0.0f),(1.0f),(1.0f),(0.5f)}} };
	//batch->Draw(
	//	m_debugSprite.handle, m_debugSprite.size, m_debugPosition, &m_srcRect,
	//	_color, 0.0f, g_XMZero, m_drawScale);
}

void Player::BulletDraw(SpriteBatch* batch)
{
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Draw(batch);
	}

	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Draw(batch);
	}

	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Draw(batch);
	}
}

void Player::UIDraw(SpriteBatch* batch)
{
	m_powerGauge.Draw(batch);

	m_bulletUI.Draw(batch);
}

void Player::Reset()
{
	m_sprite.resource.Reset();

	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Reset();
	}

	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Reset();
	}

	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Reset();
	}

	m_powerGauge.Reset();

	m_bulletUI.Reset();

	m_debugSprite.resource.Reset();
}

DirectX::SimpleMath::Vector2 Player::GetPosition()
{
	return m_position;
}

SimpleMath::Rectangle Player::GetCollider()
{
	return m_collider;
}

Player::Bullets& Player::GetBulletsStructure()
{
	return m_bullets;
}

/// <summary>
/// プレイヤーの初期位置を設定する
/// </summary>
/// <param name="screenWidth"></param>
void Player::DefaultPositionSet(const float screenWidth)
{
	if (m_isPlayer1)
	{
		m_position = DirectX::SimpleMath::Vector2(
			0.0f,
			(DXTK->Screen.Height / 2) - (s_displaySize.y / 2) - s_startPositionOffset
		);
	}
	else
	{
		m_position = DirectX::SimpleMath::Vector2(
			(DXTK->Screen.Width - s_displaySize.x),
			(DXTK->Screen.Height / 2) - (s_displaySize.y / 2) - s_startPositionOffset
		);
	}
}

/// <summary>
/// 各プレイヤーの中心に至る前進速度を計算し代入する
/// </summary>
/// <param name="screenWidth"></param>
void Player::ComeUpSpeedSet(const float screenWidth)
{
	if (m_isPlayer1)
	{
		m_bulletLaunchPositionOffset = s_bulletLaunchPositionOffsetPlayer1;

		float _dis = (screenWidth / 2) - s_comeUpSpeedSetOffsetPlayer1;//中心点までの距離計算
		m_comeUpSpeed = std::abs(_dis) / s_comeUpTime;//前進速度計算
	}
	else
	{
		m_bulletLaunchPositionOffset = s_bulletLaunchPositionOffsetPlayer2;

		float _dis = (screenWidth / 2) - s_comeUpSpeedSetOffsetPlayer2;//中心点までの距離計算
		m_comeUpSpeed = std::abs(_dis) / s_comeUpTime;//前進速度計算
	}
}