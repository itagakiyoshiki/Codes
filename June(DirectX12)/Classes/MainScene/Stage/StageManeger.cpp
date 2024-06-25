#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "StageManeger.h"

using namespace SimpleMath;

void StageManeger::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	for (int i = 0; i < s_stageCount; i++)
	{
		m_stage[i].LoadAssets(resourceUploadBatch);
	}
}

void StageManeger::Initialize()
{
	for (int i = 0; i < s_stageCount; i++)
	{
		m_stage[i].Initialize(s_initializePositionArry[i]);
	}
}

void StageManeger::OnDeviceLost()
{
	for (int i = 0; i < s_stageCount; i++)
	{
		m_stage[i].OnDeviceLost();
	}
}

void StageManeger::Render(DirectXTK::Camera& camera)
{
	for (int i = 0; i < s_stageCount; i++)
	{
		m_stage[i].Render(camera);
	}
}

