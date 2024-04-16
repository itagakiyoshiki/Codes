#pragma once
#include "Scenes/Scene.h"

class PlayerMove
{
public:
	void Initialize();

	void Move();

	void Death();

	SimpleMath::Vector3 GetPosition()
	{
		return m_position;
	}

	SimpleMath::Vector3 GetDefaultViewVector()
	{
		return s_defaultViewVector;
	}

	SimpleMath::Vector3 GetViewVector()
	{
		return m_viewVector;
	}

	enum MovingState
	{
		Hide, Out,
	};

	MovingState GetMovingState()
	{
		return m_movingState;
	}



private:

	void Update();

	static constexpr SimpleMath::Vector3 s_hidePosition =
		SimpleMath::Vector3(3.9f, 1.9f, -4.3f);

	static constexpr SimpleMath::Vector3 s_outPosiiton =
		SimpleMath::Vector3(3.6, 1.9, -4.7);

	static constexpr SimpleMath::Vector3 s_defaultViewVector =
		SimpleMath::Vector3(0.0f, 0.0f, -30);

	static constexpr SimpleMath::Vector3 s_hideViewvector =
		SimpleMath::Vector3(0.0f, -1.0f, 0);

	static constexpr SimpleMath::Vector3 s_outViewvector =
		SimpleMath::Vector3(0.0f, -1.0f, -6);

	static constexpr SimpleMath::Vector3 s_deathPosition =
		SimpleMath::Vector3(3.6f, -1.0f, -4.7f);

	static constexpr SimpleMath::Vector3 s_deathViewvector =
		SimpleMath::Vector3(-1.0f, -1.0f, -6);

	static constexpr float s_speed = 10;

	const wchar_t* s_outSeFilePass =
		L"SE/AnyConv.com__PlayerOut.wav";

	const wchar_t* s_hideSeFilePass =
		L"SE/AnyConv.com__PlayerHide.wav";

	DirectXTK::Sound m_outSe;

	DirectXTK::Sound m_hideSe;

	MovingState m_movingState;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_viewVector;
};

