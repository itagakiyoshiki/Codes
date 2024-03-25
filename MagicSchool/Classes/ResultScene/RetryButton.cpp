#include "RetryButton.h"

void RetryButton::Load(ResourceUploadBatch& esourceUpload,
	const std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_spriteFilePass,
		esourceUpload,
		resourceDescriptors.get(),
		Descriptors::Descriptor::Button);
}

void RetryButton::Initialize()
{
	m_animationStart = false;
}

void RetryButton::AnimationStart()
{

}

void RetryButton::AnimationUpdate()
{

}

bool RetryButton::AnimationEnd()
{
	//アニメーション終わってたらtrue

	return false;
}

void RetryButton::Reset()
{
	m_sprite.resource.Reset();
}

void RetryButton::Draw(SpriteBatch* batch)
{
	batch->Draw(
		m_sprite.handle, m_sprite.size, s_defaultPosition, nullptr,
		Colors::White, 0.0f, g_XMZero, s_defaultPosition, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Button / s_layerOffset);
}
