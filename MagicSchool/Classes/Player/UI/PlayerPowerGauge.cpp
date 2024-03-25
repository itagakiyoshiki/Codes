#include "PlayerPowerGauge.h"

void PlayerPowerGauge::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	const Descriptors::Descriptor descript)
{


	if (descript == Descriptors::Descriptor::Player_1)
	{
		m_isPlayer1 = true;

		m_gaugeSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_gauge1SpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.Gauge_1);

		m_frameSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_gaugeFrameSpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.GaugeFrame_1);

		m_frameBackSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_lineScaleSpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.FrameBack_1);

	}
	else
	{
		//gauge2
		m_isPlayer1 = false;

		m_gaugeSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_gauge2SpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.Gauge_2);

		m_frameSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_gaugeFrameSpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.GaugeFrame_2);

		m_frameBackSprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_lineScaleSpriteFilePass,
			esourceUpload,
			m_resourceDescriptors.get(),
			m_descriptors.FrameBack_2);
	}


}

void PlayerPowerGauge::Initialize()
{
	//Player1”ÅUI‚ÌˆÊ’u
	if (m_isPlayer1)
	{
		m_gaugePosition = DirectX::SimpleMath::Vector2(
			0.0f,
			(DXTK->Screen.Height - s_gaugeSize.y) - s_playerGaugePositionOffset
		);

		m_framePosition = m_gaugePosition;

		m_frameBackPosition = m_gaugePosition;

	}
	else
	{
		m_gaugePosition = DirectX::SimpleMath::Vector2(
			(DXTK->Screen.Width - s_gaugeSize.x),
			(DXTK->Screen.Height - s_gaugeSize.y) - s_playerGaugePositionOffset
		);

		m_framePosition = m_gaugePosition;

		m_frameBackPosition = m_gaugePosition;
	}
}

void PlayerPowerGauge::Update()
{

}

float PlayerPowerGauge::GetGaugePosition_Y()
{
	return m_gaugePosition.y;
}

void PlayerPowerGauge::Draw(SpriteBatch* batch)
{
	FrameBackSpriteDraw(batch);

	GaugeSpriteDraw(batch);

	FrameSpriteDraw(batch);
}

void PlayerPowerGauge::FrameBackSpriteDraw(SpriteBatch* batch)
{
	batch->Draw(
		m_frameBackSprite.handle,
		m_frameBackSprite.size,
		m_frameBackPosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_frameBackScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Gauge_1 / s_layerOffset);
}

void PlayerPowerGauge::GaugeSpriteDraw(SpriteBatch* batch)
{
	batch->Draw(
		m_gaugeSprite.handle,
		m_gaugeSprite.size,
		m_gaugePosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_gaugeScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Gauge_1 / s_layerOffset);
}

void PlayerPowerGauge::FrameSpriteDraw(SpriteBatch* batch)
{
	batch->Draw(
		m_frameSprite.handle,
		m_frameSprite.size,
		m_framePosition,
		nullptr, Colors::White, 0.0f, g_XMZero,
		s_frameScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Gauge_1 / s_layerOffset);
}

void PlayerPowerGauge::Reset()
{
	m_gaugeSprite.resource.Reset();
	m_frameSprite.resource.Reset();
	m_frameBackSprite.resource.Reset();
}

