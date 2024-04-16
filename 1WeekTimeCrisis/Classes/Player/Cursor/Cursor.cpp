#include "Cursor.h"

using namespace SimpleMath;

void Cursor::Load(ResourceUploadBatch& esourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_spriteFilePass,
		esourceUpload,
		resourceDescriptors,
		Descriptors::Descriptor::Cursor);
}

void Cursor::Initialize()
{
	m_position = Vector2(DXTK->Screen.Width / 2, DXTK->Screen.Height / 2);
	m_oldPosition = m_position;
}

void Cursor::Update()
{
	m_position = Vector2(m_oldPosition.x + InputSystem.Mouse.delta.x,
		m_oldPosition.y + InputSystem.Mouse.delta.y
	);
	m_oldPosition = m_position;
}

void Cursor::Reset()
{
	m_sprite.resource.Reset();
}

void Cursor::Draw(SpriteBatch* batch)
{
	m_position.x = m_position.x - m_sprite.size.x / 2;
	m_position.y = m_position.y - m_sprite.size.y / 2;

	batch->Draw(
		m_sprite.handle, m_sprite.size, m_position, nullptr,
		Colors::White, s_drawRotation, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Cursor / s_layerOffset);
}
