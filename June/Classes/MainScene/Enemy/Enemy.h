#pragma once
#include "Scenes/Scene.h"
#include "../MainProject/Base/DX/Animation.h"
#include "Classes/StateStorage.h"
#include "State/EnemyStateOrigin.h"
#include "State/EnemyNomalState.h"
#include "State/EnemyAttackState.h"
#include "State/EnemyKnockBackState.h"
#include "State/EnemyBlockState.h"
#include "Classes/ItagakiMath.h"
#include "Classes/MainScene/Weapon/Spear.h"
#include "Classes/MainScene/Weapon/Axe.h"
#include "Classes/ColliderInformation.h"

class Enemy
{
public:
	Enemy() :m_handBoneIndex(ModelBone::c_Invalid)
	{

	};
	~Enemy()
	{

	};

	void LoadAssets(ResourceUploadBatch&);

	void Initialize(SimpleMath::Vector3);

	void OnDeviceLost();

	void Update(const SimpleMath::Vector3 playerPosition,
		DirectXTK::Camera& camera,
		const bool& sperOn);

	void OnCollisionEnter(Enemy& enemy, const ColliderInformation::Collider& collider);

	void Render(DirectXTK::Camera&);

	void ChangeState(StateStorage::EnemyState);

	void AnimationChange(StateStorage::EnemyState);

	void SetWorldMatrix(SimpleMath::Matrix matrix)
	{
		m_modelWorld = matrix;
	}

	void SetPosition(SimpleMath::Vector3 position)
	{
		m_position = position;
	}

	void CounterOn()
	{
		m_nomalState.CounterOn();
	}

	void Block()
	{

	}

	void Hit()
	{
		//m_hitSe->Play();

		m_knockBackCurrentCount++;

		m_hitCoolCurrentTime = 0.0f;

		m_hitOk = false;
	}

	const bool& GetPSperOn()
	{
		return m_sperOn;
	}

	SimpleMath::Matrix& GetWorldMatrix()
	{
		return m_modelWorld;
	}

	DirectX::XMMATRIX& GetRightHandMatrix()
	{
		return m_draw_bones[m_handBoneIndex];
	}

	SimpleMath::Vector3& GetPosition()
	{
		return m_position;
	}

	SimpleMath::Vector3& GetPlayerPosition()
	{
		return m_playerPosition;
	}

	DirectX::BoundingBox& GetCollider()
	{
		return m_collider;
	}

	std::unique_ptr<Model>& GetModel()
	{
		return m_model;
	}

	EnemyAttackState& GetAttackState()
	{
		return m_attackState;
	}

	Spear& GetSpear()
	{
		return m_spear;
	}

	Axe& GetAxe()
	{
		return m_axe;
	}

	const bool& GetCounterOn()
	{
		return m_nomalState.GetCounterOn();
	}

	const bool& GetHitOk()
	{
		return m_hitOk;
	}

private:

	SimpleMath::Vector3 Cross(SimpleMath::Vector3, SimpleMath::Vector3);

	SimpleMath::Matrix ModelLookAt(SimpleMath::Matrix matrix,
		SimpleMath::Vector3 target, SimpleMath::Vector3 position);

	void AnimationUpdate();

	void AnimationTimeReset();

	void MatrixUpdate();

	void HitReactionUpdate();

	void ColliderUpdate();

	void CreateDebugModel(RenderTargetState);
	void RenderDebugModel(DirectXTK::Camera&);

	static constexpr unsigned int s_stateCount =
		static_cast<int>(StateStorage::EnemyState::Count);

	const wchar_t* s_animationFilePassArry[s_stateCount]
		= { L"Model/Enemy/JunePlayerIdel.sdkmesh_anim",
			L"Model/Enemy/JunePlayerRun.sdkmesh_anim",
			L"Model/Enemy/HurimawasiAttack.sdkmesh_anim",
			L"Model/Enemy/JunePlayerHit.sdkmesh_anim",
			L"Model/Enemy/JuneNockbackMotion.sdkmesh_anim",
			L"Model/Enemy/YousumiLeft.sdkmesh_anim",
			L"Model/Enemy/YousumiRigiht.sdkmesh_anim",
			L"Model/Player/JuneAttackMotion.sdkmesh_anim",
			L"Model/Enemy/Sword And Shield Death.sdkmesh_anim",
			L"Model/Enemy/Standing Block Idle.sdkmesh_anim"
	};

	const wchar_t* s_modelFilePass =
		L"Model/Enemy/JuneEnemyWalk.sdkmesh";

	static constexpr SimpleMath::Vector3 s_colliderSize =
		SimpleMath::Vector3(100.0f, 600.0f, 100.0f);

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	//この距離以下に入ったら攻撃する
	static constexpr float s_attackRange = 10.8f;

	//初期の大きさと初期の回転値
	static constexpr float s_defaultRotation = 0.0f * Mathf::Deg2Rad;
	static constexpr float s_defaultScale = 1.0f;//0.001

	static constexpr float s_hitReactionMaxScale = 0.001f;

	//連続で攻撃に当たらない用にするための変数
	static constexpr float s_hitCoolTIme = 1.0f;

	//ダメージリアクションの間隔の秒数
	static constexpr float s_reactionCoolTIme = 0.05f;

	//連続で攻撃受けると吹き飛ぶ数値
	static constexpr int s_knockBackCount = 2;

	//攻撃のクールタイム
	static constexpr float s_attackCoolTime = 1.5f;

	//移動開始する距離
	static constexpr float s_moveStartRange = 6.0f;

	//プレイヤーとこれ以上近づかない距離
	static constexpr float s_socialRange = 2.0f;

	DirectXTK::Sound m_hitSe;

	SimpleMath::Vector3 m_position;

	SimpleMath::Vector3 m_playerPosition;

	SimpleMath::Vector3 m_colliderCenterPosition;

	//武器
	Spear m_spear;
	Axe m_axe;

	DirectX::XMMATRIX m_handBoneMatrix;
	uint32_t m_handBoneIndex;

	//ダメージリアクション用行列
	SimpleMath::Matrix m_defaultSizeMatrix;
	SimpleMath::Matrix m_reactioningSizeMatrix;

	DirectX::BoundingBox m_collider;

	//アニメーション
	ModelBone::TransformArray m_draw_bones;

	ModelBone::TransformArray m_animation_bones;

	DX::AnimationSDKMESH m_animation[s_stateCount];

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	EnemyStateOrigin* m_currentState;
	EnemyNomalState m_nomalState;
	EnemyAttackState m_attackState;
	EnemyKnockBackState m_knockBackState;
	EnemyBlockState m_blockState;

	unsigned int m_enemyStateIndex;

	unsigned int m_knockBackCurrentCount;

	float m_hitCoolCurrentTime;

	float m_reactionCoolCurrentTime;

	float m_attackCoolCurrentTime;

	bool m_reactionNowBigger;

	bool m_hitOk;

	bool m_attackOk;

	bool m_sperOn;


	//デバッグ用モデル
	//デバック時 true にする変数
	static constexpr bool s_DebugOn = true;
	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

