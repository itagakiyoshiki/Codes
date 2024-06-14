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

	//���f���̃t�@�C���p�X
	const wchar_t* s_playerVerModelFilePass =
		L"Model/Weapon/PlainSpear.sdkmesh";

	const wchar_t* s_enemyVerModelFilePass =
		L"Model/Weapon/Enemy/PlainSpearRed.sdkmesh";

	//�R���C�_�[���s�����̎��̃T�C�Y
	static constexpr SimpleMath::Vector3 s_colliderOffSize =
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	//�R���C�_�[�̑傫��
	static constexpr SimpleMath::Vector3 s_colliderOnSize =
		SimpleMath::Vector3(50.0f, 50.0f, 50.0f);

	//�����̑傫���Ə����̉�]�l
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
	//�G�t�F�N�g�����_���[
	EffekseerRenderer::RendererRef m_efkRenderer = nullptr;
	//�G�t�F�N�g�}�l�[�W���[
	Effekseer::ManagerRef m_efkManager = nullptr;
	//Dx12��Bulkan,metal�Ȃǁ@
	// �R�}���h���X�g���g�����C�u�����ɑΉ����邽�߂̂���
	//�������v�[��
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_efkMemoryPool = nullptr;
	//�R�}���h���X�g
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_efkCmdList = nullptr;
	//�G�t�F�N�g�Đ��ɕK�v�ȕ�
	//�G�t�F�N�g�{��(�G�t�F�N�g�t�@�C���ɑΉ�)
	Effekseer::EffectRef m_effect = nullptr;
	//�G�t�F�N�g�n���h��(�Đ����̃G�t�F�N�g�ɑΉ�)
	Effekseer::Handle m_efkHandle;

	bool m_playerUse;

	//�f�o�b�O�p���f��
	//�f�o�b�N�� true �ɂ���ϐ�
	static constexpr bool s_DebugOn = true;
	void RenderDebugModel(DirectXTK::Camera&);
	void CreateDebugModel(RenderTargetState);
	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

