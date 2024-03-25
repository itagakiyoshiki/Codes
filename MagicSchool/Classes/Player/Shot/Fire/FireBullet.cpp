#include "FireBullet.h"

using namespace SimpleMath;

void FireBullet::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	const Descriptors::Descriptor descript)
{
	if (descript == Descriptors::Descriptor::Player_1)
	{
		m_isPlayer1 = true;

		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_spriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			Descriptors::Descriptor::FireBullet_1);
	}
	else
	{
		m_isPlayer1 = false;

		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_spriteReverseFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			Descriptors::Descriptor::FireBullet_2);
	}

	//“–‚½‚è”»’èÝ’è
	ColliderPositionSet();
	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

}

void FireBullet::Initialize()
{
	m_position = s_offPosition;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	m_ShootingOk = true;
}

void FireBullet::Update()
{
	if (m_ShootingOk)
	{
		return;
	}

	if (m_isPlayer1)
	{
		m_position.x += s_speed * DXTK->Time.deltaTime;

		if (m_position.x >= DXTK->Screen.Width)
		{
			m_position = s_offPosition;
			m_ShootingOk = true;
		}
	}
	else
	{
		m_position.x -= s_speed * DXTK->Time.deltaTime;

		if (m_position.x <= 0.0f)
		{
			m_position = s_offPosition;
			m_ShootingOk = true;
		}
	}

	ColliderPositionSet();
}

void FireBullet::Shoot(const SimpleMath::Vector2 shootpos)
{
	m_ShootingOk = false;
	m_position = shootpos;
}

void FireBullet::Hit()
{
	m_hitSe->Play();
	m_position = s_offPosition;
	m_ShootingOk = true;
}


void FireBullet::Reset()
{
	m_sprite.resource.Reset();
}

void FireBullet::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, nullptr,
		Colors::White, 0.0f, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::FireBullet_1 / s_layerOffset);
}

void FireBullet::ColliderPositionSet()
{
	m_collider.x = m_position.x;
	m_collider.y = m_position.y;
}

bool FireBullet::GetShootingOK()
{
	return m_ShootingOk;
}

SimpleMath::Rectangle FireBullet::GetCollider()
{
	return m_collider;
}
