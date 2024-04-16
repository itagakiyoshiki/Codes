#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Player.h"

void Player::LoadAssets(ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_cursor.Load(resourceUpload, resourceDescriptors);

	m_bullet.LoadAssets(resourceUpload);

}

void Player::Initialize()
{
	m_playerMove.Initialize();

	m_cursor.Initialize();

	m_bullet.Initialize();

	m_isDeath = false;

	m_shotSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_deathSeFilePass);
}

void Player::Update(DirectXTK::Camera camera)
{
	if (m_isDeath)
	{
		m_playerMove.Death();
	}
	else
	{
		m_playerMove.Move();

		m_cursor.Update();
	}

	if (InputSystem.Mouse.isPressed.leftButton
		&& m_playerMove.GetMovingState() == PlayerMove::MovingState::Out
		&& !m_isDeath)
	{
		m_bullet.Fire(camera);

	}

	m_bullet.Update(camera);

}

void Player::Death()
{
	m_isDeath = true;

	m_shotSe->Play();
}

void Player::Reset()
{
	m_cursor.Reset();

	m_bullet.OnDeviceLost();
}

/// <summary>
/// 照準用の画像を読み込む関数
/// </summary>
/// <param name="batch"></param>
void Player::SpriteRender(SpriteBatch* batch)
{
	m_cursor.Draw(batch);
}

/// <summary>
/// 打ち出す弾用のモデルを読み込む関数
/// </summary>
/// <param name="camera"></param>
void Player::ModelRender(DirectXTK::Camera camera)
{
	m_bullet.Render(camera);
}
