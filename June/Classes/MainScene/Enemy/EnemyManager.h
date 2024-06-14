#pragma once
#include "Scenes/Scene.h"
#include "Enemy.h"

class EnemyManager
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void OnDeviceLost();

	void Update(SimpleMath::Vector3);

	void Render(DirectXTK::Camera&);

	struct EnemyManagerStruct
	{
	public:
		static constexpr int s_enemyCount = 5;

		Enemy& GetEnemy(int num)
		{
			return m_enemy[num];
		}

	private:
		Enemy m_enemy[s_enemyCount];

	};

	EnemyManagerStruct& GetEnemyManagerStruct()
	{
		return m_enemyManagerStruct;
	}

private:

	static constexpr SimpleMath::Vector3 s_initializePositionArry[EnemyManagerStruct::s_enemyCount]
		= {
		SimpleMath::Vector3(34.0f, 0.5f, 0.0f),
		SimpleMath::Vector3(-12.0f, 0.2f, 0.0f),
		SimpleMath::Vector3(5.0f, 0.0f, 0.0f),
		SimpleMath::Vector3(17.0f, 0.8f, 0.0f),
		SimpleMath::Vector3(-28.0f, 0.1f, 0.0f)
	};

	EnemyManagerStruct m_enemyManagerStruct;

};

