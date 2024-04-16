#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "PlayerMove.h"

using namespace SimpleMath;

void PlayerMove::Initialize()
{
	m_position = s_hidePosition;
	m_movingState = MovingState::Hide;

	m_outSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_outSeFilePass);

	m_hideSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hideSeFilePass);
}

void PlayerMove::Move()
{
	if (InputSystem.Keyboard.wasPressedThisFrame.Space)
	{
		if (m_movingState == MovingState::Hide)
		{
			m_movingState = MovingState::Out;
			m_outSe->Play();
		}
		else
		{
			m_movingState = MovingState::Hide;
			m_hideSe->Play();
		}
	}

	Update();
}

void PlayerMove::Death()
{
	m_position = s_deathPosition;

	m_viewVector = s_deathViewvector;
}

void PlayerMove::Update()
{
	if (m_movingState == PlayerMove::Hide)
	{
		Vector3 _hideVec = s_hidePosition - m_position;
		m_position += (_hideVec * s_speed) * DXTK->Time.deltaTime;

		m_viewVector = s_hideViewvector;
	}
	else
	{
		Vector3 _outVec = s_outPosiiton - m_position;
		m_position += (_outVec * s_speed) * DXTK->Time.deltaTime;

		m_viewVector = s_outViewvector;
	}

}
