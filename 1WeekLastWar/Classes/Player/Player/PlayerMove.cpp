#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "PlayerMove.h"

using namespace SimpleMath;

Vector3 PlayerMove::PositionInitialize(const Vector3 position)
{
	Vector3 _nowPosition = position;

	_nowPosition = s_defaultPositon;

	return _nowPosition;
}

SimpleMath::Vector3 PlayerMove::RotationInitialize(const Vector3 rotation)
{
	Vector3 _nowRotation = rotation;

	_nowRotation = Vector3(0, 0, -1);

	return _nowRotation;
}

SimpleMath::Matrix PlayerMove::MatrixInitialize(const Matrix matrix, const Vector3 position)
{
	Matrix _nowMatrix = matrix;

	_nowMatrix = Matrix::CreateRotationX(s_modelDefaultRotation * Mathf::Deg2Rad);

	_nowMatrix._41 = position.x; _nowMatrix._42 = position.y; _nowMatrix._43 = position.z;

	return _nowMatrix;
}

Vector3 PlayerMove::PositionUpdate(const Vector3 position)
{
	Vector3 _nowPosition = position;

	if (InputSystem.Keyboard.isPressed.Left)
	{
		_nowPosition.x += -s_speed * DXTK->Time.deltaTime;
	}
	if (InputSystem.Keyboard.isPressed.Right)
	{
		_nowPosition.x += s_speed * DXTK->Time.deltaTime;
	}

	return _nowPosition;
}

SimpleMath::Vector3 PlayerMove::RotationUpdate(const Vector3 rotation)
{
	Vector3 _nowRotation = rotation;

	return _nowRotation;
}

SimpleMath::Matrix PlayerMove::MatrixUpdate(const Matrix matrix, const Vector3 position)
{
	Matrix _nowMatrix = matrix;

	_nowMatrix._41 = position.x; _nowMatrix._42 = position.y; _nowMatrix._43 = position.z;

	return _nowMatrix;
}
