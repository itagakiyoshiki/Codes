#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "BulletManeger.h"

using namespace SimpleMath;

void BulletManeger::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		m_bulletStruct.m_bullet[i].LoadAssets(resourceUploadBatch);
	}
}

void BulletManeger::Initialize(SimpleMath::Vector3 position)
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		m_bulletStruct.m_bullet[i].Initialize(position);
	}
}

void BulletManeger::Update()
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		m_bulletStruct.m_bullet[i].Update();
	}
}

void BulletManeger::Fire(SimpleMath::Vector3 position)
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		if (m_bulletStruct.m_bullet[i].GetBulletStruct().GetFireOn())
		{
			continue;
		}
		m_bulletStruct.m_bullet[i].Fire(position);
		break;
	}
}

void BulletManeger::OnDeviceLost()
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		m_bulletStruct.m_bullet[i].OnDeviceLost();
	}
}

void BulletManeger::Render(DirectXTK::Camera& camera)
{
	for (int i = 0; i < m_bulletStruct.s_bulletCount; i++)
	{
		m_bulletStruct.m_bullet[i].Render(camera);
	}
}