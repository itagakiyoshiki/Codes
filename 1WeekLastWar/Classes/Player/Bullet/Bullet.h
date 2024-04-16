#pragma once
#include "Scenes/Scene.h"

class Bullet
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize(SimpleMath::Vector3);

	void Update();

	void Fire(SimpleMath::Vector3);

	void Hit();

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	struct BulletStruct
	{
	private:
		SimpleMath::Vector3 m_position;

		DirectX::BoundingBox m_collider;

		bool m_fireOn;

	public:
		void SetPosition(SimpleMath::Vector3 positon)
		{
			m_position = positon;
		}

		void SetFireOnFlag(bool flag)
		{
			m_fireOn = flag;
		}

		void SetCollider(DirectX::BoundingBox collider)
		{
			m_collider = collider;
		}

		SimpleMath::Vector3 GetPosition()
		{
			return m_position;
		}

		DirectX::BoundingBox GetCollider()
		{
			return m_collider;
		}

		bool GetFireOn()
		{
			return m_fireOn;
		}

	};

	BulletStruct& GetBulletStruct()
	{
		return m_bulletStruct;
	}

private:

	void MatrixUpdate();

	void CreateDebugModel(RenderTargetState);

	void ColliderUpdate();

	void RenderDebugModel(SimpleMath::Matrix, SimpleMath::Matrix);

	const wchar_t* s_ModelFilePass =
		L"Models/Player/Bullet/EnemyBullet1Week.cmo";

	const wchar_t* s_shotSeFilePass =
		L"Sound/SE/AnyConv.com__PlayerShot.wav";

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	static constexpr float s_rePopTime = 1.5f;
	float m_rePopCurrentTime;

	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(1, 1, 1);

	static constexpr SimpleMath::Vector3 s_offPositon =
		SimpleMath::Vector3(0, 100.0f, 0);

	static constexpr bool s_DebugOn = true;

	SimpleMath::Matrix m_worldMatrix;

	BulletStruct m_bulletStruct;

	DirectXTK::Sound m_shotSe;

	DirectXTK::Sound m_hitSe;

	SimpleMath::Vector3 m_colliderCenterPosition;

	std::unique_ptr<CommonStates> m_commonStates;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

