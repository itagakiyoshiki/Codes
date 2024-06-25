#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "WeaponOrigin.h"

using namespace SimpleMath;

WeaponOrigin::WeaponOrigin()
{

}

void WeaponOrigin::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{

}

void WeaponOrigin::Initialize()
{

}

void WeaponOrigin::OnDeviceLost()
{

}

void WeaponOrigin::Update(SimpleMath::Vector3 position, StateStorage::MoveState movestate)
{

}

void WeaponOrigin::Render(DirectXTK::Camera& camera)
{

}

/// <summary>
/// OnDeviceLost()Ç≈åƒÇ—èoÇ∑ä÷êî
/// </summary>
void WeaponOrigin::ModelOnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}
