#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "WeaponRack.h"

using namespace SimpleMath;

void WeaponRack::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	m_weaponStruct.GetSpear().LoadAssets(resourceUploadBatch);
}

void WeaponRack::Initialize()
{
	m_weaponState = WeaponState::NONE;

	m_weaponStruct.GetSpear().Initialize();
}

void WeaponRack::OnDeviceLost()
{
	m_weaponStruct.GetSpear().OnDeviceLost();
}

void WeaponRack::Attack(const StateStorage::AttackState attackMode)
{
	switch (attackMode)
	{
	case StateStorage::AttackState::None:

		break;
	case StateStorage::AttackState::Spear:
		m_weaponState = WeaponState::SPEAR;
		SpearAttackStart();
		break;
	default:
		break;
	}
}

void WeaponRack::SpearAttackStart()
{
	m_weaponStruct.GetSpear().AttackStart();
}

void WeaponRack::Update(SimpleMath::Vector3 position, StateStorage::MoveState moveState)
{
	m_weaponStruct.GetSpear().Update(position, moveState);
}

void WeaponRack::Render(DirectXTK::Camera& camera)
{
	m_weaponStruct.GetSpear().Render(camera);
}

