#pragma once
#include "Scenes/Scene.h"

class EnemyBullet
{
public:
	void LoadAssets(ResourceUploadBatch&);
	//EnemyBullet1Week.cmo
	void Initialize();

	void Fire(SimpleMath::Vector3);

	void Update(DirectX::XMMATRIX,
		DirectX::XMMATRIX, SimpleMath::Vector3);

	void Hit(DirectXTK::Camera& camera);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	SimpleMath::Vector3 GetPosition()
	{
		return m_position;
	}

	bool GetFireOn()
	{
		return m_fireOn;
	}

private:
	void FireOff(SimpleMath::Vector3);

	SimpleMath::Vector3 Cross(
		SimpleMath::Vector3, SimpleMath::Vector3);

	const wchar_t* s_shotSeFilePass =
		L"SE/AnyConv.com__EnemyShot.wav";

	static constexpr SimpleMath::Vector3 s_defaultPosition =
		SimpleMath::Vector3(0, 3, -2);

	static constexpr SimpleMath::Vector3 s_defaultRotation =
		SimpleMath::Vector3(0, 0, 0);

	static constexpr float s_rePopTime = 2.0f;

	static constexpr float s_speed = 10.0f;

	DirectXTK::Sound m_shotSe;

	SimpleMath::Vector3 m_targetPositon;

	SimpleMath::Vector3 m_bulletRotation;

	SimpleMath::Vector3 m_bulletZdir;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_rotation;

	SimpleMath::Vector3 m_oldRotation;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	float m_rePopCurrentTime;

	bool m_fireOn;
};

