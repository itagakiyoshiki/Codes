#pragma once
#include "Scenes/Scene.h"

class Stage
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void Initialize(SimpleMath::Vector3);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

private:

	const wchar_t* s_modelFilePass =
		L"Model/Stage/Road/1WeekRoad.sdkmesh";

	static constexpr float s_defaultRotationZ = 180.0f;
	static constexpr float s_defaultScale = 100.0f;

	SimpleMath::Vector3 m_position;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

};

