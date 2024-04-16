#pragma once
#include "Scenes/Scene.h"
#include "Item.h"

class ItemManeger
{
public:

	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void Update();

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	struct ItemMangerStruct
	{
		static constexpr int s_itemCount = 3;

		Item m_item[s_itemCount];
	};

	ItemMangerStruct& GetItemStruct()
	{
		return m_itemManegerStruct;
	}
private:
	static constexpr int s_launchPositonArrayCount = 10;
	static constexpr SimpleMath::Vector3 s_launchPositonArray[s_launchPositonArrayCount]
		= {
			SimpleMath::Vector3(6, 0, 9),
			SimpleMath::Vector3(-1, 0, 7),
			SimpleMath::Vector3(3, 0, 7),
			SimpleMath::Vector3(-4, 0, 7),
			SimpleMath::Vector3(4, 0, 8),
			SimpleMath::Vector3(1, 0, 10),
			SimpleMath::Vector3(-2, 0, 9),
			SimpleMath::Vector3(-5, 0, 7),
			SimpleMath::Vector3(2, 0, 8),
			SimpleMath::Vector3(-6, 0, 7)
	};

	static constexpr SimpleMath::Vector3 s_offPositon =
		SimpleMath::Vector3(0, 100.0f, 0);

	static constexpr float s_popCoolTime = 5.0f;
	float m_popCurrentTime;

	int m_itemLaunchIndex;
	ItemMangerStruct m_itemManegerStruct;
};

