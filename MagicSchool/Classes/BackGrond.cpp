#include "BackGrond.h"

void BackGrond::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors,
	const Descriptors::Descriptor descriptors)
{
	if (descriptors == Descriptors::Descriptor::BG)//main”wŒi
	{
		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_battleBgFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			descriptors);
	}
	else											//result”wŒi
	{
		m_sprite = DirectXTK::CreateSpriteSRV(
			DXTK->Device,
			s_resultBgFilePass,
			esourceUpload,
			resourceDescriptors.get(),
			descriptors);
	}
}

void BackGrond::Reset()
{
	m_sprite.resource.Reset();
}

void BackGrond::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_sprite.handle, m_sprite.size, s_defaultPosition, nullptr,
		Colors::White, 0.0f, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::BG / s_layerOffset);
}

