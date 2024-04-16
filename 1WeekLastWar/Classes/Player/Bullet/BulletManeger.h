#pragma once
#include "Scenes/Scene.h"
#include "Bullet.h"

class BulletManeger
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize(SimpleMath::Vector3);

	void Update();

	void Fire(SimpleMath::Vector3);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	struct BulletSturct
	{
		static constexpr int s_bulletCount = 20;

		Bullet m_bullet[s_bulletCount];
	};

	BulletSturct& GetBUlletStruct()
	{
		return m_bulletStruct;
	}

private:

	BulletSturct m_bulletStruct;

};

