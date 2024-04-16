#pragma once
#include "Scenes/Scene.h"

class Stage
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void OnDeviceLost();

	void Render(DirectXTK::Camera);

private:

	const wchar_t* s_modelFilePass =
		L"Model/1WeekStage.cmo";

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

};

