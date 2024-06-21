#pragma once
#include "Scenes/Scene.h"
#include "Classes/ItagakiMath.h"

class Enemy;

class Axe
{
public:
	void LoadAssets(ResourceUploadBatch& resourceUploadBatch, const bool playerUse);

	void Initialize();

	void OnDeviceLost();

	void Render(DirectXTK::Camera& camera);

	void ColliderOn()
	{
		m_collider.Extents = s_colliderOnSize;
	}

	void ColliderOff()
	{
		m_collider.Extents = s_colliderOffSize;
	}

	void SetPosition(SimpleMath::Vector3 position)
	{
		m_position = position;
		ColliderUpdate();

		ModelMatrixUpdate();
	}

	void EnemyHandFollowUpdate(SimpleMath::Matrix handMat, SimpleMath::Matrix parentworldMat);

	void SetAxeTheta(const float theta)
	{
		m_axeTheta = theta;
	}

	DirectX::BoundingBox& GetCollider()
	{
		return m_collider;
	}

	SimpleMath::Vector3& GetPosition()
	{
		return m_position;
	}

	SimpleMath::Matrix& GetModelWorldMatrix()
	{
		return m_modelWorld;
	}

private:

	void ColliderUpdate();

	void ModelMatrixUpdate();

	SimpleMath::Matrix ModelLookAt(SimpleMath::Vector3 target, SimpleMath::Vector3 position);

	//モデルのファイルパス
	const wchar_t* s_playerVerModelFilePass =
		L"Model/Weapon/JuneAxe.sdkmesh";
	const wchar_t* s_enemyVerModelFilePass =
		L"Model/Weapon/Enemy/JuneAxeRed.sdkmesh";

	//コライダーが不活性の時のサイズ
	static constexpr SimpleMath::Vector3 s_colliderOffSize =
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	//コライダーの大きさ
	static constexpr SimpleMath::Vector3 s_colliderOnSize =
		SimpleMath::Vector3(50.0f, 50.0f, 50.0f);

	//ローカル初期回転値
	static constexpr SimpleMath::Vector3 s_defaultRotation =
		SimpleMath::Vector3(
			180.0f * Mathf::Deg2Rad,
			80.0f * Mathf::Deg2Rad,
			330.0f * Mathf::Deg2Rad);
	//ローカル初期位置
	static constexpr SimpleMath::Vector3 s_defaultPosition =
		SimpleMath::Vector3(-75.0f, 140.0f, 6.0f);

	//初期の大きさと初期の回転値
	static constexpr float s_defaultRotationZ = 90.0f * Mathf::Deg2Rad;
	static constexpr float s_defaultScale = 1.0f;

	DirectX::BoundingBox m_collider;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_colliderCenterPosition;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	bool m_playerUse;

	float m_axeTheta;

	//デバッグ用モデル
	//デバック時 true にする変数
	static constexpr bool s_DebugOn = false;
	void RenderDebugModel(DirectXTK::Camera&);
	void CreateDebugModel(RenderTargetState);
	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

