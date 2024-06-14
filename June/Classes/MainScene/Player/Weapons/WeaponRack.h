#pragma once
#include "Scenes/Scene.h"
#include "Spear.h"
#include "Classes/StateStorage.h"

class WeaponRack
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void OnDeviceLost();

	void Attack(const StateStorage::AttackState);

	void Update(SimpleMath::Vector3, StateStorage::MoveState);

	void Render(DirectXTK::Camera&);

	struct WeaponStruct
	{
	public:

		Spear& GetSpear()
		{
			return m_spear;
		}

	private:

		Spear m_spear;

	};

	WeaponStruct& GetWeaponStruct()
	{
		return m_weaponStruct;
	}

private:
	const enum WeaponState
	{
		NONE, SPEAR
	};


	void SpearAttackStart();

	WeaponStruct m_weaponStruct;

	WeaponState m_weaponState;



};

