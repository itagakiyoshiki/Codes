#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"
#include "Classes/ItagakiMath.h"

class PlayerCamera
{
public:
	void LoadAssets();

	void Update(SimpleMath::Matrix);

	void Render();

	DirectXTK::Camera& GetCamera()
	{
		return m_camera;
	}

private:


	static constexpr SimpleMath::Vector3 s_defaultPositon =
		SimpleMath::Vector3(0, 350.0f, -1500.0f);

	static constexpr float m_rotationSpeed = 1.0f;

	float s_cameraHorizontalSpeed = 300.0f;

	DirectXTK::Camera m_camera;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_rotation;

	float m_cameraDistance;

	float m_cameraAngle;

};

