#pragma once
#include "Scenes/Scene.h"

class Item
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void Update();

	void Hit();

	void Launch(SimpleMath::Vector3);

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	struct ItemStruct
	{
	private:
		SimpleMath::Vector3 m_position;

		DirectX::BoundingBox m_collider;

		bool m_inGame;

	public:
		void SetPosition(SimpleMath::Vector3 positon)
		{
			m_position = positon;
		}

		void SetCollider(DirectX::BoundingBox collider)
		{
			m_collider = collider;
		}

		void InGameOn()
		{
			m_inGame = true;
		}

		void InGameOff()
		{
			m_inGame = false;
		}

		bool GetInGame()
		{
			return m_inGame;
		}

		SimpleMath::Vector3 GetPosition()
		{
			return m_position;
		}

		DirectX::BoundingBox GetCollider()
		{
			return m_collider;
		}

	};

	ItemStruct& GetItemStruct()
	{
		return m_itemStruct;
	}

private:

	void MatrixUpdate();

	void ColliderUpdate();

	void CreateDebugModel(RenderTargetState);

	void RenderDebugModel(SimpleMath::Matrix, SimpleMath::Matrix);

	static constexpr float s_modelScale = 0.25f;

	static constexpr float s_screenOutPosiiton = -12.0f;

	static constexpr float s_speed = 3.0f;

	const wchar_t* s_ModelFilePass =
		L"Models/Item/sward.sdkmesh";

	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(1, 1, 1);

	static constexpr SimpleMath::Vector3 s_offPositon =
		SimpleMath::Vector3(0, 100.0f, 0);

	SimpleMath::Vector3 m_rotation;

	SimpleMath::Matrix m_worldMatrix;

	ItemStruct m_itemStruct;

	SimpleMath::Vector3 m_colliderCenterPosition;

	static constexpr bool s_DebugOn = true;

	std::unique_ptr<CommonStates> m_commonStates;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

