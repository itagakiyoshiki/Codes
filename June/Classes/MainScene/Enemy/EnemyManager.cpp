#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyManager.h"

using namespace SimpleMath;

void EnemyManager::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	for (int i = 0; i < m_enemyManagerStruct.s_enemyCount; i++)
	{
		m_enemyManagerStruct.GetEnemy(i).LoadAssets(resourceUploadBatch);
	}
}

void EnemyManager::Initialize()
{
	for (int i = 0; i < m_enemyManagerStruct.s_enemyCount; i++)
	{
		m_enemyManagerStruct.GetEnemy(i).Initialize(s_initializePositionArry[i]);
	}
}

void EnemyManager::OnDeviceLost()
{
	for (int i = 0; i < m_enemyManagerStruct.s_enemyCount; i++)
	{
		m_enemyManagerStruct.GetEnemy(i).OnDeviceLost();
	}
}

void EnemyManager::Update(Vector3 playerPosition)
{
	for (int i = 0; i < m_enemyManagerStruct.s_enemyCount; i++)
	{
		m_enemyManagerStruct.GetEnemy(i).Update(playerPosition);
	}
}

void EnemyManager::Render(DirectXTK::Camera& camera)
{
	for (int i = 0; i < m_enemyManagerStruct.s_enemyCount; i++)
	{
		m_enemyManagerStruct.GetEnemy(i).Render(camera);
	}
}

