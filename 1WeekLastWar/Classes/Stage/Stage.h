#pragma once
#include "Scenes/Scene.h"

class Stage
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

private:

	void ModelLoad(ResourceUploadBatch&);

	const wchar_t* s_ModelFilePass =
		L"Models/Road/1WeekRoad.sdkmesh";

	SimpleMath::Vector3 m_position;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;


};

