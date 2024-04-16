#pragma once
#include "Scenes/Scene.h"

class Bullet
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void Fire(DirectXTK::Camera);

	void Update(DirectXTK::Camera);

	void Hit(DirectXTK::Camera& camera);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	SimpleMath::Vector3 GetPosition()
	{
		return m_position;
	}

private:

	void FireOff(DirectXTK::Camera& camera);

	SimpleMath::Vector3 Cross(
		SimpleMath::Vector3, SimpleMath::Vector3);

	const wchar_t* s_modelFilePass =
		L"Model/Bullet1week.cmo";

	const wchar_t* s_shotSeFilePass =
		L"SE/AnyConv.com__PlayerShot.wav";

	static constexpr SimpleMath::Vector3 s_defaultPosition =
		SimpleMath::Vector3(0, 3, -2);

	static constexpr float s_speed = 10.0f;

	static constexpr float s_rePopTime = 2.0f;

	DirectXTK::Sound m_shotSe;

	SimpleMath::Vector3 m_viewPositon;

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

	SimpleMath::Matrix m_viewPortMatrix;

	float m_rePopCurrentTime;

	bool m_fireOn;

};

