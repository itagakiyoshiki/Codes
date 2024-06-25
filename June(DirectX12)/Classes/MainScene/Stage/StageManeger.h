#pragma once
#include "Scenes/Scene.h"
#include "Stage.h"

class StageManeger
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

private:

	static constexpr int s_stageCount = 5;

	static constexpr SimpleMath::Vector3 s_initializePositionArry[s_stageCount]
		= {
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		SimpleMath::Vector3(20.0f,0.0f,0.0f),
		SimpleMath::Vector3(-20.0f,0.0f,0.0f),
		SimpleMath::Vector3(40.0f,0.0f,0.0f),
		SimpleMath::Vector3(-40.0f,0.0f,0.0f),
	};

	Stage m_stage[s_stageCount];

};

