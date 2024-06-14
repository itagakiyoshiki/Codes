#pragma once
#include "Scenes/Scene.h"
#include "WeaponOrigin.h"
#include "Classes/StateStorage.h"

class Spear : WeaponOrigin
{
public:
	Spear();

	~Spear()
	{
		OnDeviceLost();
	}

	void LoadAssets(ResourceUploadBatch&) override;

	void Initialize() override;

	void OnDeviceLost() override;

	void Update(SimpleMath::Vector3, StateStorage::MoveState) override;

	void Render(DirectXTK::Camera&) override;

	void AttackStart();

	bool GetAttackOk()
	{
		return m_attackOk;
	}

	DirectX::BoundingBox& GetCollider()
	{
		return m_collider;
	}

private:

	const enum HorizonState
	{
		Left, Right
	};

	void ModelLoad(ResourceUploadBatch&);

	void ModelOnDeviceLost();

	void PositionUpdate(SimpleMath::Vector3, StateStorage::MoveState);

	void FlagUpdate();

	void CreateDebugModel(RenderTargetState);

	void ColliderUpdate();

	void RenderDebugModel(DirectXTK::Camera&);

	const wchar_t* s_ModelFilePass =
		L"Models/Player/Weapons/1WeekWideActionSpear.sdkmesh";

	static constexpr SimpleMath::Vector3 s_rightPosition =
		SimpleMath::Vector3(1, 0.5f, 0);

	static constexpr SimpleMath::Vector3 s_leftPosition =
		SimpleMath::Vector3(-1, 0.5f, 0);

	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(1, 1, 1);

	//初期の大きさと初期の回転値
	static constexpr float s_defaultRotationZ = 90.0f;
	static constexpr float s_defaultScale = 0.015f;

	//攻撃コライダーを出す時間
	static constexpr float s_attackTime = 1.0f;

	//デバック時 true にする変数
	static constexpr bool s_DebugOn = true;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_colliderCenterPosition;

	SimpleMath::Matrix m_leftModeMatrix;
	SimpleMath::Matrix m_rigthModeMatrix;

	DirectX::BoundingBox m_collider;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	HorizonState m_horizonState;

	float m_attackCurrentTime;

	bool m_attackOk;

	//デバッグ用モデル
	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

