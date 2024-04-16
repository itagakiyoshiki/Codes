#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyManeger.h"

using namespace SimpleMath;

void EnemyManeger::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
	{
		m_enemyManegerSturct.m_enemy[i].LoadAssets(resourceUploadBatch);
	}
}

void EnemyManeger::Initialize()
{
	for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
	{
		m_enemyManegerSturct.m_enemy[i].GetEnemyStruct().SetPosition(s_offPositon);
		m_enemyManegerSturct.m_enemy[i].Initialize();
	}

	m_enemyLaunchIndex = 0;

	m_popCurrentTime = 0.0f;
}

void EnemyManeger::Update()
{
	m_popCurrentTime += DXTK->Time.deltaTime;
	if (m_popCurrentTime >= s_popCoolTime)
	{
		m_popCurrentTime = 0.0f;
		for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
		{
			if (!m_enemyManegerSturct.m_enemy[i].GetEnemyStruct().GetInGame())
			{
				m_enemyManegerSturct.m_enemy[i].Launch(s_launchPositonArray[m_enemyLaunchIndex]);

				m_enemyLaunchIndex++;
				if (m_enemyLaunchIndex >= s_launchPositonArrayCount)
				{
					m_enemyLaunchIndex = 0;
				}
				break;
			}
		}
	}

	for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
	{
		m_enemyManegerSturct.m_enemy[i].Update();
	}
}

void EnemyManeger::OnDeviceLost()
{
	for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
	{
		m_enemyManegerSturct.m_enemy[i].OnDeviceLost();
	}
}

void EnemyManeger::Render(DirectXTK::Camera& camera)
{
	for (int i = 0; i < m_enemyManegerSturct.s_enemyCount; i++)
	{
		m_enemyManegerSturct.m_enemy[i].Render(camera);
	}
}
