#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "PlayerCamera.h"

using namespace SimpleMath;

void PlayerCamera::LoadAssets()
{
	m_position = s_defaultPositon;
	m_rotation = Vector3(50.0f, 0.8f, 0.0f);

	m_camera.SetView(
		m_position,
		m_rotation);

	m_camera.SetPerspectiveFieldOfView(
		Mathf::PI / 4.0f,
		(float)DXTK->Screen.Width / (float)DXTK->Screen.Height,
		0.01f, 10000.0f);

	m_rotation = Vector3(0, 0, 0);

	m_cameraAngle = -90.0f;

	m_cameraDistance = 850.0f;
}

void PlayerCamera::Update(Matrix playerMatrix)
{

	if (InputSystem.Keyboard.isPressed.Up)
	{
		m_position.y += 10.0f;
	}
	else if (InputSystem.Keyboard.isPressed.Down)
	{
		m_position.y += -10.0f;
	}

	if (InputSystem.Keyboard.isPressed.Left)
	{
		m_cameraAngle += s_cameraHorizontalSpeed * DXTK->Time.deltaTime;
		if (m_cameraAngle >= 360.0f)
		{
			m_cameraAngle += -360.0f;
		}
	}
	else if (InputSystem.Keyboard.isPressed.Right)
	{
		m_cameraAngle += -s_cameraHorizontalSpeed * DXTK->Time.deltaTime;
		if (m_cameraAngle <= -360.0f)
		{
			m_cameraAngle += 360.0f;
		}
	}

	if (InputSystem.Keyboard.isPressed.Y)
	{
		m_cameraDistance += 100.0f * DXTK->Time.deltaTime;
	}
	if (InputSystem.Keyboard.isPressed.H)
	{
		m_cameraDistance += -100.0f * DXTK->Time.deltaTime;
	}

	//プレイヤーの位置取得
	Vector3 _playerPosition;
	_playerPosition.x = playerMatrix._41;
	_playerPosition.y = playerMatrix._42;
	_playerPosition.z = playerMatrix._43;

	//プレイヤーの前方ベクトル取得
	Vector3 _playerForwardVector = playerMatrix.Forward();
	_playerForwardVector.y = 0.0f;
	_playerForwardVector.Normalize();

	float _camerax = _playerPosition.x + cos(m_cameraAngle * Mathf::Deg2Rad) * m_cameraDistance;
	float _cameraz = _playerPosition.z + sin(m_cameraAngle * Mathf::Deg2Rad) * m_cameraDistance;

	m_position.x = _camerax;
	m_position.z = _cameraz;

	m_camera.SetViewLookAt(m_position, _playerPosition, Vector3::Up);


}

void PlayerCamera::Render()
{

}

