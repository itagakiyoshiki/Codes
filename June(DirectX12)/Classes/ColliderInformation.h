#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"

class ColliderInformation
{
public:

	struct Collider
	{
		Collider()
		{
			_hitPosition = SimpleMath::Vector3::Zero;
			_hitWeaponState = StateStorage::WeaponState::None;
		}

		SimpleMath::Vector3 _hitPosition;
		StateStorage::WeaponState _hitWeaponState;
	};

private:


};

