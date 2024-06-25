#pragma once
#include "Scenes/Scene.h"

class StateStorage
{
public:
	const enum class PlayerState
	{
		Stay, Move, Attack, Hit,
		Count
	};

	const enum class WeaponState
	{
		None, Spear, Axe,
		Count
	};

	const enum class EnemyState
	{
		Stay, Move, HUrimawasiAttack, Hit,
		NockBack, WaitSeeRight, WaitSeeLeft,
		ShotAttack, Counter, Block,
		Count
	};

private:


};

