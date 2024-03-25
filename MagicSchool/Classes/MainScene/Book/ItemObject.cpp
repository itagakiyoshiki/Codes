#include "ItemObject.h"

void ItemObject::Load(ResourceUploadBatch& esourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_spriteFilePass,
		esourceUpload,
		resourceDescriptors.get(),
		Descriptors::Descriptor::Item);

	//“–‚½‚è”»’èÝ’è
	m_position = s_offPosition;

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;
	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

}

void ItemObject::Initialize()
{
	m_ShootingOk = false;
}

void ItemObject::Update()
{
	if (!m_ShootingOk)
	{
		return;
	}

	if (m_isPlayer1)
	{
		m_position.x += -s_speed * DXTK->Time.deltaTime;

		if (m_position.x >= DXTK->Screen.Width)
		{
			m_position = s_offPosition;
			m_ShootingOk = false;
		}
	}
	else
	{
		m_position.x += s_speed * DXTK->Time.deltaTime;

		if (m_position.x <= 0.0f)
		{
			m_position = s_offPosition;
			m_ShootingOk = false;
		}
	}

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;

}

void ItemObject::Pop(DirectX::SimpleMath::Vector2 popPos, bool player1)
{
	m_position = popPos;

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;

	m_ShootingOk = true;

	m_isPlayer1 = player1;
}

void ItemObject::Hit()
{
	m_hitSe->Play();

	m_ShootingOk = false;

	m_position = s_offPosition;

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;
}

void ItemObject::Reset()
{
	m_sprite.resource.Reset();
}

void ItemObject::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, nullptr,
		Colors::White, 0.0f, g_XMZero, s_drawScale);
}

SimpleMath::Rectangle ItemObject::GetCollider()
{
	return m_collider;
}

