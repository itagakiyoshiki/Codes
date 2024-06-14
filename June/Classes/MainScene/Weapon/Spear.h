#pragma once
#include "Scenes/Scene.h"
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#include "Classes/ItagakiMath.h"

#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"LLGI.lib")

class Spear
{
public:
	void LoadAssets(ResourceUploadBatch& resourceUploadBatch, const bool playerUse);

	void Initialize();

	void OnDeviceLost();

	void Render(DirectXTK::Camera& camera);

	void OnCollisionEnter();

	void ColliderOn()
	{
		m_collider.Extents = s_colliderOnSize;
	}

	void ColliderOff()
	{
		m_collider.Extents = s_colliderOffSize;
	}

	void SetPosition(const SimpleMath::Vector3 position);

	void SetTargetPosition(const SimpleMath::Vector3 position);

	const SimpleMath::Vector3& GetColliderSize()
	{
		return m_collider.Extents;
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

	void EffectLoad();

	void EffectUpdate(DirectXTK::Camera& camera);

	void ColliderUpdate();

	void ModelMatrixUpdate();

	SimpleMath::Matrix ModelLookAt(SimpleMath::Vector3 target, SimpleMath::Vector3 position);

	//モデルのファイルパス
	const wchar_t* s_playerVerModelFilePass =
		L"Model/Weapon/PlainSpear.sdkmesh";

	const wchar_t* s_enemyVerModelFilePass =
		L"Model/Weapon/Enemy/PlainSpearRed.sdkmesh";

	//コライダーが不活性の時のサイズ
	static constexpr SimpleMath::Vector3 s_colliderOffSize =
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	//コライダーの大きさ
	static constexpr SimpleMath::Vector3 s_colliderOnSize =
		SimpleMath::Vector3(50.0f, 50.0f, 50.0f);

	//初期の大きさと初期の回転値
	static constexpr float s_defaultRotationZ = 90.0f * Mathf::Deg2Rad;
	static constexpr float s_defaultScale = 1.0f;

	static constexpr float s_targetVectorOffset = 10.0f;

	DirectX::BoundingBox m_collider;

	SimpleMath::Vector3 m_position;
	SimpleMath::Vector3 m_colliderCenterPosition;
	SimpleMath::Vector3 m_targetVector;
	SimpleMath::Vector3 m_targetPosition;

	std::unique_ptr<CommonStates> m_commonStates;
	SimpleMath::Matrix m_modelWorld;
	std::unique_ptr<Model> m_model;
	Model::EffectCollection m_modelNomal;
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<EffectFactory> m_effectFactory;

	//Effect
	static constexpr float s_effectScale = 20.0f;
	//エフェクトレンダラー
	EffekseerRenderer::RendererRef m_efkRenderer = nullptr;
	//エフェクトマネージャー
	Effekseer::ManagerRef m_efkManager = nullptr;
	//Dx12やBulkan,metalなど　
	// コマンドリストを使うライブラリに対応するためのもの
	//メモリプール
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_efkMemoryPool = nullptr;
	//コマンドリスト
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_efkCmdList = nullptr;
	//エフェクト再生に必要な物
	//エフェクト本体(エフェクトファイルに対応)
	Effekseer::EffectRef m_effect = nullptr;
	//エフェクトハンドル(再生中のエフェクトに対応)
	Effekseer::Handle m_efkHandle;

	bool m_playerUse;

	//デバッグ用モデル
	//デバック時 true にする変数
	static constexpr bool s_DebugOn = true;
	void RenderDebugModel(DirectXTK::Camera&);
	void CreateDebugModel(RenderTargetState);
	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

