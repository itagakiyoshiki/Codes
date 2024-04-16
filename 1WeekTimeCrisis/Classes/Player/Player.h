#pragma once
#include "Scenes/Scene.h"
#include <Audio.h>
#include "Classes/Descriptors .h"
#include "Classes/Player/PlayerMove.h"
#include "Classes/Player/Cursor/Cursor.h"
#include "Classes/Player/Bullet/Bullet.h"

class Player
{
public:
	void LoadAssets(ResourceUploadBatch&,
		std::unique_ptr<DirectX::DescriptorHeap>&);

	void Initialize();

	void Update(DirectXTK::Camera);

	void Reset();

	void SpriteRender(SpriteBatch*);

	void ModelRender(DirectXTK::Camera);

	void BulletHit(DirectXTK::Camera camera)
	{
		m_bullet.Hit(camera);
	}

	void Death();

	SimpleMath::Vector3 GetPosition()
	{
		return m_playerMove.GetPosition();
	}

	SimpleMath::Vector3 GetBulletPosition()
	{
		return m_bullet.GetPosition();
	}

	SimpleMath::Vector3 GetDefaultViewVector()
	{
		return m_playerMove.GetDefaultViewVector();
	}

	SimpleMath::Vector3 GetViewVector()
	{
		return m_playerMove.GetViewVector();
	}

	PlayerMove::MovingState GetMovingState()
	{
		return m_playerMove.GetMovingState();
	}

	bool GetDeath()
	{
		return m_isDeath;
	}

private:

	const wchar_t* s_deathSeFilePass =
		L"SE/AnyConv.com__PlayerDeath.wav";

	DirectXTK::Sound m_shotSe;

	PlayerMove m_playerMove;

	Cursor m_cursor;

	Bullet m_bullet;

	bool m_isDeath;

};

