#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"

class Player;

class StateOrigin
{
public:

	virtual void Initialize();

	virtual void OnEnter(Player& owner, StateOrigin& beforState);

	virtual void OnUpdate(Player&, const DirectXTK::Camera& camera);

	virtual void OnExit(Player& owner, StateOrigin& nextState);

	virtual void OnCollisionEnter();

private:


};

