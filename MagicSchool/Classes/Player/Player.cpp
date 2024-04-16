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

	//�v���C���[�摜�ǂݍ���
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		fileName.c_str(),
		esourceUpload,
		resourceDescriptors.get(),
		descriptors);

	//�f�o�b�O�p�摜�̓ǂݍ���
	m_debugSprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_debugSpriteFilePass,
		esourceUpload,
		resourceDescriptors.get(),
		_descript);

	//�e�̉摜�̓ǂݍ���
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

	//UI�̉摜�̓ǂݍ���
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

	//�v���C���[�̏����ʒu��ݒ肷��
	DefaultPositionSet(DXTK->Screen.Width);

	//��ʒ����Ɋ���Ă��������ʃt���O
	m_comeUpOk = true;
	m_cuurentTime = 0.0f;

	//�e�v���C���[�̒��S�Ɏ���O�i���x���v�Z���������
	ComeUpSpeedSet(DXTK->Screen.Width);

	//�����̃��[�h��ҋ@�ɂ��đ��삳���܂ł��̂܂܂�y���̈ʒu�ɋ���
	moveMode = Player::MoveMode::Stay;

	//�����蔻��ݒ�
	ColliderPositionSet();
	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

	//�f�o�b�O�p�摜�ݒ�
	DebugSpritePositionSet();

	//�e
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

	//���ꂼ��̒e�̃N�[���_�E���p�^�C�}�[������
	//�e��ł��Ă������t���O�̏�����
	//��
	m_fireCurrentTime = 0.0f;
	m_fireShotOk = true;
	//�X
	m_iceCurrentTime = 0.0f;
	m_iceShotOk = true;
	//��
	m_windCurrentTime = 0.0f;
	m_windShotOk = true;

	//�Q�[�W��UI�̏�����
	m_powerGauge.Initialize();

	//�e��UI�̏�����
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
/// ������󂯕t���ē������[�h��؂�ւ���
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
/// �����̓����X�e�[�g�ɂ�蓮������ς���
/// </summary>
void Player::MoveUpdate()
{
	DirectX::SimpleMath::Vector2 direction = DirectX::SimpleMath::Vector2::Zero;

	//�㉺�^��
	if (moveMode == Player::MoveMode::Up)
	{
		direction.y += -s_verticalitySpeed * m_speed;
	}
	else if (moveMode == Player::MoveMode::Down)
	{
		direction.y += s_verticalitySpeed * m_speed;
	}

	//���S�Ɋ��^��
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

	//���S�Ɋ���Ă�����Ȃ�
	if (m_cuurentTime >= s_comeUpTime && m_comeUpOk)
	{
		m_comeUpOk = false;
	}
	else
	{
		m_cuurentTime += DXTK->Time.deltaTime;
	}

	//�ړ������s
	m_position += direction;

	//��ʊO�ɏo�Ȃ��悤�Ɉʒu����
	m_position.x = std::clamp(
		m_position.x,
		0.0f, float(DXTK->Screen.Width - s_displaySize.x)
	);
	m_position.y = std::clamp(
		m_position.y,
		0.0f, m_powerGauge.GetGaugePosition_Y() - s_heightClampOffset
	);

	//�R���C�_�[�ʒu����
	ColliderPositionSet();

	//�f�o�b�O�p�摜�ʒu����
	DebugSpritePositionSet();

	//�e�p�̔��ˈʒu�̒���
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
		//��
		if (InputSystem.Keyboard.wasPressedThisFrame.Z)
		{
			FireShot();
		}

		//�X
		if (InputSystem.Keyboard.wasPressedThisFrame.X
			&& m_havingItemCount >= s_iceBulletAuthorizationItemCount)
		{
			IceShot();
		}

		//��
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

	//�ʒu�X�V
	ShootUpdate();
}

/// <summary>
/// �����ĂȂ��΂�����
/// </summary>
void Player::FireShot()
{
	//�΂̒e�𔭎ˉ\�ɂȂ��Ă����猂��
	if (!m_fireShotOk)
	{
		return;
	}

	//�΂̒e���������̂Ŕ��˕s�ɂ��Ĕ��ˉ\�����f�ɗ���
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
/// �����ĂȂ��X������
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
/// �����ĂȂ���������
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
/// �e�e�̈ʒu�𓮂���
/// </summary>
void Player::ShootUpdate()
{
	//��
	for (int i = 0; i < m_bullets.s_fireCount; i++)
	{
		m_bullets.m_fireBullet[i].Update();
	}

	//�X
	for (int i = 0; i < m_bullets.s_iceCount; i++)
	{
		m_bullets.m_iceBullet[i].Update();
	}

	//��
	for (int i = 0; i < m_bullets.s_windCount; i++)
	{
		m_bullets.m_windBullet[i].Update();
	}
}

/// <summary>
/// �e�e�̔��˃N�[���^�C�����v��
/// </summary>
void Player::ShootTimeUpdate()
{
	//��
	if (!m_fireShotOk)
	{
		m_fireCurrentTime += DXTK->Time.deltaTime;
		if (m_fireCurrentTime >= s_fireCoolTime)
		{
			m_fireCurrentTime = 0.0f;
			m_fireShotOk = true;
		}
	}

	//�X
	if (!m_iceShotOk)
	{
		m_iceCurrentTime += DXTK->Time.deltaTime;
		if (m_iceCurrentTime >= s_iceCoolTime)
		{
			m_iceCurrentTime = 0.0f;
			m_iceShotOk = true;
		}
	}

	//��
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

	// �ҋ@�A�j���[�V����
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
	//�v���C���[�摜�����o��
	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, &m_srcRect,
		Colors::White, 0.0f, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Player_1 / s_layerOffset);

	//�f�o�b�O�p�摜�����o��
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
/// �v���C���[�̏����ʒu��ݒ肷��
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
/// �e�v���C���[�̒��S�Ɏ���O�i���x���v�Z���������
/// </summary>
/// <param name="screenWidth"></param>
void Player::ComeUpSpeedSet(const float screenWidth)
{
	if (m_isPlayer1)
	{
		m_bulletLaunchPositionOffset = s_bulletLaunchPositionOffsetPlayer1;

		float _dis = (screenWidth / 2) - s_comeUpSpeedSetOffsetPlayer1;//���S�_�܂ł̋����v�Z
		m_comeUpSpeed = std::abs(_dis) / s_comeUpTime;//�O�i���x�v�Z
	}
	else
	{
		m_bulletLaunchPositionOffset = s_bulletLaunchPositionOffsetPlayer2;

		float _dis = (screenWidth / 2) - s_comeUpSpeedSetOffsetPlayer2;//���S�_�܂ł̋����v�Z
		m_comeUpSpeed = std::abs(_dis) / s_comeUpTime;//�O�i���x�v�Z
	}
}