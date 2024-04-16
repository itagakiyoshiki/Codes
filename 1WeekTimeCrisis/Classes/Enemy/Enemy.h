#pragma once
#include "Scenes/Scene.h"
#include "EnemyBullet.h"

class Enemy
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void Update(DirectXTK::Camera, bool);

	void Hit();

	void OnDeviceLost();

	void Render(DirectXTK::Camera);

	SimpleMath::Vector3 GetPosition()
	{
		return m_position;
	}

	SimpleMath::Vector3 GetBulletPosition()
	{
		return m_enemyBullet.GetPosition();
	}

	bool GetIsDeath()
	{
		return m_isDeath;
	}

private:
	static constexpr int s_clearLineCount = 6;

	static constexpr int s_popPositionCount = 8;
	static constexpr SimpleMath::Vector3 s_popPosition[s_popPositionCount]
		= {
		SimpleMath::Vector3(-2.5f ,2, -3),
		SimpleMath::Vector3(-1 ,2.7f, -3),
		SimpleMath::Vector3(0, 3, -2),
		SimpleMath::Vector3(-4.0f ,2, 2.5),
		SimpleMath::Vector3(2, 3, -2),
		SimpleMath::Vector3(-4.0f ,2, -5),
		SimpleMath::Vector3(1.6, 1, -1.8),
		SimpleMath::Vector3(1.6, 10, -1.8)
	};

	SimpleMath::Vector3 Cross(
		SimpleMath::Vector3, SimpleMath::Vector3);

	const wchar_t* s_modelFilePass =
		L"Model/Bullet1week.cmo";

	const wchar_t* s_hitSeFilePass =
		L"SE/AnyConv.com__EnemyHit.wav";

	static constexpr SimpleMath::Vector3 s_outPosiiton =
		SimpleMath::Vector3(3.6, 1.9, -4.7);

	static constexpr SimpleMath::Vector3 s_defaultRotation =
		SimpleMath::Vector3(0, 0, 0);

	static constexpr float s_speed = 100.0f;

	static constexpr int s_shootCoolTimeArrayCount = 5;
	static constexpr float s_shootCoolTimeArray[s_shootCoolTimeArrayCount] =
	{
		1.0f,4.6f,3.0f,0.8f,2.0f
	};



	DirectXTK::Sound m_hitSe;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_rotation;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	SimpleMath::Matrix m_viewPortMatrix;

	EnemyBullet m_enemyBullet;

	int m_popIndex;

	float m_shootCooltime;
	float m_shootCoolCurrentTime;
	int m_shootCoolTimeIndex;


	bool m_isDeath;

};

