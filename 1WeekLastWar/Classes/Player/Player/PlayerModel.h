#pragma once
#include "Scenes/Scene.h"

class PlayerModel
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&, SimpleMath::Matrix);

private:

	void ModelLoad(ResourceUploadBatch&);



	const wchar_t* s_ModelFilePass =
		L"Models/Player/Player/1WeekPlayer.sdkmesh";



	std::unique_ptr<CommonStates> m_commonStates;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

};

