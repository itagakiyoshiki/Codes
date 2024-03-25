#include<DirectXMath.h>
#include "WindBullet.h"

using namespace SimpleMath;

void WindBullet::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors,
	const Descriptors::Descriptor descript)
{
	if (descript == Descriptors::Descriptor::Player_1)
	{
		m_isPlayer1 = true;

		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_spriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			Descriptors::Descriptor::WindBullet_1);
	}
	else
	{
		m_isPlayer1 = false;

		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_spriteReverseFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			Descriptors::Descriptor::WindBullet_2);
	}

	//“–‚½‚è”»’èÝ’è
	ColliderPositionSet();
	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

}

void WindBullet::Initialize()
{
	m_position = s_offPosition;

	m_launchPosition = s_offPosition;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	m_signCount = 0.0f;

	m_ShootingOk = true;
}

void WindBullet::Update()
{
	if (m_ShootingOk)
	{
		return;
	}

	if (m_isPlayer1)
	{
		m_position.x += s_speed * DXTK->Time.deltaTime;
		m_position.y =
			m_launchPosition.y + std::sin(DirectX::XM_2PI / s_upDownSpeed * m_signCount) * s_waveRange;
		m_signCount++;

		if (m_position.x >= DXTK->Screen.Width)
		{
			m_signCount = 0;
			m_position = s_offPosition;
			m_ShootingOk = true;
		}
	}
	else
	{
		m_position.x -= s_speed * DXTK->Time.deltaTime;
		m_position.y =
			m_launchPosition.y - std::sin(DirectX::XM_2PI / s_upDownSpeed * m_signCount) * s_waveRange;
		m_signCount++;

		if (m_position.x <= 0.0f)
		{
			m_signCount = 0;
			m_position = s_offPosition;
			m_ShootingOk = true;
		}
	}

	ColliderPositionSet();
}

void WindBullet::Shoot(const SimpleMath::Vector2 shootpos)
{
	m_ShootingOk = false;
	m_position = shootpos;
	m_launchPosition = shootpos;
}

void WindBullet::Hit()
{
	m_hitSe->Play();
	m_position = s_offPosition;
	m_launchPosition = s_offPosition;
	m_ShootingOk = true;
}


void WindBullet::Reset()
{
	m_sprite.resource.Reset();
}

void WindBullet::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, nullptr,
		Colors::White, 0.0f, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::WindBullet_2 / s_layerOffset);
}

void WindBullet::ColliderPositionSet()
{
	m_collider.x = m_position.x;
	m_collider.y = m_position.y;
}

bool WindBullet::GetShootingOK()
{
	return m_ShootingOk;
}

SimpleMath::Rectangle WindBullet::GetCollider()
{
	return m_collider;
}
