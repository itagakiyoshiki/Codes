#pragma once
#include "Scenes/Scene.h"
#include "StateOrigin.h"

class Player;

class MoveState : public StateOrigin
{
public:

	void Initialize(Player&);

	void OnEnter(Player&, StateOrigin&);

	void OnUpdate(Player&, const DirectXTK::Camera&);

	void WeponFollowUpdate(Player& player);

	void OnExit(Player&, StateOrigin&);

	void OnCollisionEnter();

	const enum MovingState
	{
		None, Move, Forward, Right, Left, Back,
	};

	SimpleMath::Vector3& GetMoveVector()
	{
		return m_moveVector;
	}

private:

	void MoveVectorUpdate(Player& player, const DirectXTK::Camera& camera);

	void PositionUpdate(Player& player, const DirectXTK::Camera& camera);

	//武器とプレイヤーをどれだけ離すか
	static constexpr float s_weponDistance = 100.0f;
	//武器と地面をどれだけ離すか
	static constexpr float s_weponUpDistance = 100.0f;
	//斧の回転値の調整値
	static constexpr float s_axeDefaultRotationOffset = 90.0f * Mathf::Deg2Rad;

	static constexpr float s_speed = 1000.0f;

	static constexpr SimpleMath::Vector3 s_defaultPositon =
		SimpleMath::Vector3(0, 0, 0);

	//移動キーを押したときのカメラのベクトル
	SimpleMath::Vector3 m_moveStartCameraForwardVector;
	SimpleMath::Vector3 m_moveStartCameraRightVector;

	SimpleMath::Vector3 m_moveVector;

	size_t m_movingIndex;
	size_t m_oldMovingIndex;

	bool m_moveNow;


};

