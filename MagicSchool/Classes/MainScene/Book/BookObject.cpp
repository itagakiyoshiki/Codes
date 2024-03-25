#include "BookObject.h"

void BookObject::Load(ResourceUploadBatch& esourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_spriteFilePass,
		esourceUpload,
		resourceDescriptors.get(),
		Descriptors::Descriptor::Book);
}

void BookObject::Initialize()
{

	m_centerLinePosition = DXTK->Screen.Width / 2 - s_centerLinePositionOffset;

	m_position =
		DirectX::SimpleMath::Vector2(m_centerLinePosition, 0.0f);

	m_lowerLimit = DXTK->Screen.Height - s_lowerLimitOffset;

	m_moveState = MoveState::Up;

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;

	m_collider.height = s_displaySize.x;
	m_collider.width = s_displaySize.y;

	m_rePopCurrentTime = 0.0f;

	m_bulletHit = false;

	m_rePopOk = false;

}

void BookObject::Update()
{
	//弾に当たって数秒経ったらリポップ
	//その前にプレイヤーが取得していた場合はやらない
	if (m_bulletHit)
	{
		m_position = s_screenOffPosition;

		m_rePopCurrentTime += DXTK->Time.deltaTime;

		if (m_rePopCurrentTime >= s_rePopTime)
		{
			RePop();
			m_bulletHit = false;
			m_rePopOk = false;
		}

		return;
	}

	if (m_moveState == MoveState::Up)
	{
		m_position.y += -s_speed * DXTK->Time.deltaTime;
	}
	else
	{
		m_position.y += s_speed * DXTK->Time.deltaTime;
	}

	m_collider.x = m_position.x;
	m_collider.y = m_position.y;


	if (m_position.y >= m_lowerLimit)
	{
		m_moveState = MoveState::Up;
	}
	else if (m_position.y <= s_upperLimit)
	{
		m_moveState = MoveState::Down;
	}
}

void BookObject::Hit()
{
	if (!m_bulletHit)
	{
		m_rePopCurrentTime = 0.0f;

		m_bulletHit = true;
		m_rePopOk = true;

		m_collider.x = s_screenOffPosition.x;
		m_collider.y = s_screenOffPosition.y;
	}
}

void BookObject::RePop()
{
	if (m_rePopOk)
	{

		m_position =
			DirectX::SimpleMath::Vector2(m_centerLinePosition, 0.0f);

		m_collider.x = m_position.x;
		m_collider.y = m_position.y;

		m_bulletHit = false;
	}
}

void BookObject::Reset()
{
	m_sprite.resource.Reset();
}

void BookObject::Draw(SpriteBatch* batch)
{
	batch->Draw(m_sprite.handle, m_sprite.size, m_position);
}

SimpleMath::Vector2 BookObject::GetPosition()
{
	return m_position;
}

SimpleMath::Rectangle BookObject::GetCollider()
{
	return m_collider;
}


