#pragma once
#include "Scenes/Scene.h"
#include "PlayerModel.h"
#include "PlayerMove.h"
#include "Classes/Player/Bullet/BulletManeger.h"

class Player
{
public:
	void LoadAssets(ResourceUploadBatch&);

	void Initialize();

	void Update();

	void ItemGet();

	void OnDeviceLost();

	void Render(DirectXTK::Camera&);

	SimpleMath::Vector3 GetPosition()
	{
		return m_position;
	}

	BulletManeger::BulletSturct& GetBulletSturct()
	{
		return m_bulletManeger.GetBUlletStruct();
	}

	DirectX::BoundingBox GetCollider()
	{
		return m_collider;
	}

private:

	void ColliderUpdate();

	void CreateDebugModel(RenderTargetState);

	void RenderDebugModel(DirectXTK::Camera&, SimpleMath::Matrix);

	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(1, 1, 1);

	const wchar_t* s_itemGetSeFilePass =
		L"Sound/SE/AnyConv.com__PlayerOut.wav";

	DirectXTK::Sound m_itemGetSe;

	DirectX::BoundingBox m_collider;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_rotation;

	SimpleMath::Matrix m_worldMatrix;

	PlayerModel m_playerModel;

	PlayerMove m_playerMove;

	BulletManeger m_bulletManeger;

	float m_shotCoolTime;
	float m_shotCoolCurrentTime;



	SimpleMath::Vector3 m_colliderCenterPosition;

	static constexpr bool s_DebugOn = true;

	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

