#include "PlayerBulletUI.h"

void PlayerBulletUI::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors,
	const Descriptors::Descriptor descript)
{
	if (descript == Descriptors::Descriptor::Player_1)
	{
		m_isPlayer1 = true;

		m_fireSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_fireSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.FireBulletUI_1);

		m_iceSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_iceSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.IceBulletUI_1);

		m_windSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_windSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.WindBulletUI_1);

		m_hiddenSprite_1 = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_unknownSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.HiddenBullet_1);

		m_hiddenSprite_2 = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_unknownSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.HiddenBullet_2);
	}
	else
	{
		m_isPlayer1 = false;

		m_fireSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_fireSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.FireBulletUI_2);

		m_iceSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_iceSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.IceBulletUI_2);

		m_windSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_windSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.WindBulletUI_2);

		m_hiddenSprite_1 = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_unknownSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.HiddenBullet_1);

		m_hiddenSprite_2 = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_unknownSpriteFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			m_descriptors.HiddenBullet_2);
	}

}

void PlayerBulletUI::Initialize()
{
	//Player1版UIの位置
	if (m_isPlayer1)
	{
		m_firePosition = DirectX::SimpleMath::Vector2(
			0.0f,
			(DXTK->Screen.Height - s_displaySize.y)
		);
		m_icePosition = DirectX::SimpleMath::Vector2(
			m_firePosition.x + s_displaySize.x,
			m_firePosition.y
		);
		m_windPosition = DirectX::SimpleMath::Vector2(
			m_icePosition.x + s_displaySize.x,
			m_firePosition.y
		);
		m_hidden_1Position = DirectX::SimpleMath::Vector2(
			m_icePosition.x,
			m_icePosition.y
		);
		m_hidden_2Position = DirectX::SimpleMath::Vector2(
			m_windPosition.x,
			m_windPosition.y
		);
	}
	else
	{
		m_firePosition = DirectX::SimpleMath::Vector2(
			(DXTK->Screen.Width - s_displaySize.x),
			(DXTK->Screen.Height - s_displaySize.y)
		);
		m_icePosition = DirectX::SimpleMath::Vector2(
			m_firePosition.x - s_displaySize.x,
			m_firePosition.y
		);
		m_windPosition = DirectX::SimpleMath::Vector2(
			m_icePosition.x - s_displaySize.x,
			m_firePosition.y
		);
		m_hidden_1Position = DirectX::SimpleMath::Vector2(
			m_icePosition.x,
			m_icePosition.y
		);
		m_hidden_2Position = DirectX::SimpleMath::Vector2(
			m_windPosition.x,
			m_windPosition.y
		);
	}

	m_havingItemCount = 0;
}

void PlayerBulletUI::Update()
{

}

void PlayerBulletUI::ItemHit()
{
	m_havingItemCount++;

	if (m_havingItemCount >= s_iceBulletAuthorizationItemCount)
	{
		m_hidden_1Position = s_offPosition;
	}

	if (m_havingItemCount >= s_windBulletAuthorizationItemCount)
	{
		m_hidden_2Position = s_offPosition;
	}

}

void PlayerBulletUI::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_fireSprite.handle,
		m_fireSprite.size,
		m_firePosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::FireBulletUI_1 / s_layerOffset);

	batch->Draw(
		m_iceSprite.handle,
		m_iceSprite.size,
		m_icePosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::IceBulletUI_1 / s_layerOffset);

	batch->Draw(
		m_windSprite.handle,
		m_windSprite.size,
		m_windPosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::WindBulletUI_1 / s_layerOffset);

	batch->Draw(
		m_hiddenSprite_1.handle,
		m_hiddenSprite_1.size,
		m_hidden_1Position,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::HiddenBullet_1 / s_layerOffset);

	batch->Draw(
		m_hiddenSprite_2.handle,
		m_hiddenSprite_2.size,
		m_hidden_2Position,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::HiddenBullet_1 / s_layerOffset);
}

void PlayerBulletUI::Reset()
{
	m_fireSprite.resource.Reset();
	m_iceSprite.resource.Reset();
	m_windSprite.resource.Reset();
	m_hiddenSprite_1.resource.Reset();
	m_hiddenSprite_2.resource.Reset();
}

float PlayerBulletUI::GetDrawScale()
{
	return s_drawScale;
}

/// <summary>
/// Descriptorにより個別にポジションを取得できる関数
/// </summary>
/// <param name="desc"></param>
/// <returns></returns>
DirectX::SimpleMath::Vector2 PlayerBulletUI::GetPosition(Descriptors::Descriptor desc)
{
	switch (desc)
	{
	case Descriptors::BG:
		break;
	case Descriptors::Player_1:
		break;
	case Descriptors::Player_2:
		break;
	case Descriptors::Gauge_1:
		break;
	case Descriptors::Gauge_2:
		break;
	case Descriptors::FireBulletUI_1:
		return m_firePosition;
		break;
	case Descriptors::FireBulletUI_2:
		return m_firePosition;
		break;
	case Descriptors::IceBulletUI_1:
		return m_icePosition;
		break;
	case Descriptors::IceBulletUI_2:
		return m_icePosition;
		break;
	case Descriptors::WindBulletUI_1:
		return m_windPosition;
		break;
	case Descriptors::WindBulletUI_2:
		return m_windPosition;
		break;
	case Descriptors::HiddenBullet_1:
		return m_hidden_1Position;
		break;
	case Descriptors::HiddenBullet_2:
		return m_hidden_1Position;
		break;
	case Descriptors::Count:
		break;
	default:
		break;
	}
}

/// <summary>
/// m_spriteBatch->Draw()のfor文で使用する関数
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
DirectX::SimpleMath::Vector2 PlayerBulletUI::GetPosition(int index)
{
	switch (index)
	{
	case 0:
		return m_firePosition;
		break;
	case 1:
		return m_icePosition;
		break;
	case 2:
		return m_windPosition;
		break;
	case 3:
		return m_hidden_1Position;
		break;
	case 4:
		return m_hidden_2Position;
		break;
	default:
		break;
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE PlayerBulletUI::GetHandle(int index)
{
	switch (index)
	{
	case 0:
		return m_fireSprite.handle;
		break;
	case 1:
		return m_iceSprite.handle;
		break;
	case 2:
		return m_windSprite.handle;
		break;
	case 3:
		return m_hiddenSprite_1.handle;
		break;
	case 4:
		return m_hiddenSprite_2.handle;
		break;
	default:
		break;
	}
}

DirectX::XMUINT2 PlayerBulletUI::GetSize(int index)
{
	switch (index)
	{
	case 0:
		return m_fireSprite.size;
		break;
	case 1:
		return m_iceSprite.size;
		break;
	case 2:
		return m_windSprite.size;
		break;
	case 3:
		return m_hiddenSprite_1.size;
		break;
	case 4:
		return m_hiddenSprite_2.size;
		break;
	default:
		break;
	}
}