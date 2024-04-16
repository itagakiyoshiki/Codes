#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "ItemManeger.h"

using namespace SimpleMath;

void ItemManeger::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
	{
		m_itemManegerStruct.m_item[i].LoadAssets(resourceUploadBatch);
	}
}

void ItemManeger::Initialize()
{
	for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
	{
		m_itemManegerStruct.m_item[i].GetItemStruct().SetPosition(s_offPositon);
		m_itemManegerStruct.m_item[i].Initialize();
	}

	m_itemLaunchIndex = 0;

	m_popCurrentTime = 0.0f;
}

void ItemManeger::Update()
{
	m_popCurrentTime += DXTK->Time.deltaTime;
	if (m_popCurrentTime >= s_popCoolTime)
	{
		m_popCurrentTime = 0.0f;
		for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
		{
			if (!m_itemManegerStruct.m_item[i].GetItemStruct().GetInGame())
			{
				m_itemManegerStruct.m_item[i].Launch(s_launchPositonArray[m_itemLaunchIndex]);

				m_itemLaunchIndex++;
				if (m_itemLaunchIndex >= s_launchPositonArrayCount)
				{
					m_itemLaunchIndex = 0;
				}
				break;
			}
		}
	}

	for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
	{
		m_itemManegerStruct.m_item[i].Update();
	}
}

void ItemManeger::OnDeviceLost()
{
	for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
	{
		m_itemManegerStruct.m_item[i].OnDeviceLost();
	}
}

void ItemManeger::Render(DirectXTK::Camera& camera)
{
	for (int i = 0; i < m_itemManegerStruct.s_itemCount; i++)
	{
		m_itemManegerStruct.m_item[i].Render(camera);
	}
}