#pragma once
#include "Scenes/Scene.h"
#include "Classes/StateStorage.h"

class WeaponOrigin
{
public:
	WeaponOrigin();

	~WeaponOrigin()
	{
		OnDeviceLost();
	}

	virtual void LoadAssets(ResourceUploadBatch&);

	virtual void Initialize();

	virtual void OnDeviceLost();

	virtual void Update(SimpleMath::Vector3, StateStorage::MoveState);

	virtual void Render(DirectXTK::Camera&);

private:

	void ModelLoad(ResourceUploadBatch&);

	void ModelOnDeviceLost();

	void ModelRender(DirectXTK::Camera&);

	void PositionUpdate();

	void MatrixUpdate();

	const wchar_t* s_ModelFilePass =
		L"Models/Player/Weapons/1WeekWideActionSpear.sdkmesh";

	SimpleMath::Vector3 m_position;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;
};

