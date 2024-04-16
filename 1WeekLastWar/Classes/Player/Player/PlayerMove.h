#pragma once
#include "Scenes/Scene.h"

class PlayerMove
{
public:

	SimpleMath::Vector3 PositionInitialize(const SimpleMath::Vector3);

	SimpleMath::Vector3 RotationInitialize(const SimpleMath::Vector3);

	SimpleMath::Matrix MatrixInitialize(const SimpleMath::Matrix,
		const SimpleMath::Vector3);

	SimpleMath::Vector3 PositionUpdate(const SimpleMath::Vector3);

	SimpleMath::Vector3 RotationUpdate(const SimpleMath::Vector3);

	SimpleMath::Matrix MatrixUpdate(const SimpleMath::Matrix,
		const SimpleMath::Vector3);

private:

	static constexpr float s_modelDefaultRotation = 67.0f;

	static constexpr SimpleMath::Vector3 s_defaultPositon =
		SimpleMath::Vector3(0, 0.1f, -7);

	static constexpr float s_speed = 10;

};

