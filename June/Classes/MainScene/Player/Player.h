#pragma once
#include "Scenes/Scene.h"
#include "Base/DX/Animation.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "State/StateOrigin.h"
#include "State/MoveState.h"
#include "State/AttackState.h"
#include "Classes/MainScene/CounterManager.h"
#include "Classes/MainScene/Weapon/Spear.h"
#include "Classes/MainScene/Weapon/Axe.h"
#include "Classes/ItagakiMath.h"

class Player
{
public:
	Player() {};
	~Player() {};

	void LoadAssets(ResourceUploadBatch&, DirectXTK::Camera&);

	void Initialize();

	void OnDeviceLost();

	void Update(const DirectXTK::Camera&, CounterManager&, Enemy&);

	void Render(DirectXTK::Camera&);

	void ChangeState(StateStorage::PlayerState);

	void AnimationChange(StateStorage::PlayerState);

	void OnCollisionEnter();

	void SetWorldMatrix(SimpleMath::Matrix matrix)
	{
		m_modelWorld = matrix;
	}

	void SpearOnCollisionEnter()
	{
		GetSpear().OnCollisionEnter();
		GetSpear().ColliderOff();
	}

	/// <summary>
	/// �����̈ʒu�Ȃǂ�ۊǂ���\����
	/// </summary>
	struct PlayerMoveStruct
	{
	public:

		void SetMoveState(const StateStorage::PlayerState setState)
		{
			m_moveState = setState;
		}

		StateStorage::PlayerState& GetMoveState()
		{
			return m_moveState;
		}

		void SetPosition(const SimpleMath::Vector3 position)
		{
			m_position = position;
		}

		SimpleMath::Vector3& GetPosition()
		{
			return m_position;
		}

	private:
		SimpleMath::Vector3 m_position;

		StateStorage::PlayerState m_moveState;

	};

	PlayerMoveStruct& GetPlayerMoveStruct()
	{
		return m_playerMoveStruct;
	}

	AttackState& GetAttackState()
	{
		return m_attackState;
	}

	StateOrigin* GetState()
	{
		return m_currentState;
	}

	SimpleMath::Matrix& GetWorldMatrix()
	{
		return m_modelWorld;
	}

	SimpleMath::Matrix& GetBeforModelWorld()
	{
		return m_beforModelWorld;
	}

	SimpleMath::Vector3& GetMostNearEnemyPosition()
	{
		return m_nearEnemyPosition;
	}

	DirectX::BoundingBox& GetCollider()
	{
		return m_collider;
	}

	Spear& GetSpear()
	{
		return m_spear;
	}

	Axe& GetAxe()
	{
		return m_axe;
	}

private:

	SimpleMath::Matrix ModelLookAt(SimpleMath::Vector3 target, SimpleMath::Vector3 position);
	SimpleMath::Matrix ModelAttackLookAt(SimpleMath::Vector3 target, SimpleMath::Vector3 position);

	void AnimationUpdate();

	void AnimationTimeReset();

	void ModelMatrixUpdate(const DirectXTK::Camera&);

	void ModelMatrixPositionUpdate();

	void ModelMatrixRotationUpdate(const DirectXTK::Camera&);

	void MoveStartCameraMatrixSet(const DirectXTK::Camera& camera);

	//�R���C�_�[�̑傫��
	static constexpr SimpleMath::Vector3 s_colliderOnSize =
		SimpleMath::Vector3(50.0f, 300.0f, 50.0f);

	//�R���C�_�[���s�����̎��̈ʒu
	static constexpr SimpleMath::Vector3 s_colliderOffSize =
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	//�A���ōU���ɓ�����Ȃ��p�ɂ��邽�߂̕ϐ�
	static constexpr float s_hitCoolTIme = 1.0f;

	static constexpr unsigned int s_stateCount =
		static_cast<int>(StateStorage::PlayerState::Count);

	const wchar_t* s_animationFilePassArry[s_stateCount] = {
		L"Model/Player/JunePlayerIdel.sdkmesh_anim",
		L"Model/Player/JunePlayerRun.sdkmesh_anim",
		L"Model/Player/JuneAttackMotion.sdkmesh_anim",
		L"Model/Player/JunePlayerHit.sdkmesh_anim"
	};

	const wchar_t* s_modelFilePass =
		L"Model/Player/JunePlayerWalk.sdkmesh";

	//���f���̉�]�傫���̐ݒ�
	static constexpr float s_defaultScale = 1.0f;
	static constexpr float s_forwardRotation = 0.0f * Mathf::Deg2Rad;
	static constexpr float s_rightRotation = 90.0f * Mathf::Deg2Rad;
	static constexpr float s_leftRotation = -90.0f * Mathf::Deg2Rad;
	static constexpr float s_backRotation = 180.0f * Mathf::Deg2Rad;
	static constexpr float s_speed = 10.0f;

	//�U����������������SE�̃t�@�C���p�X
	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/AnyConv.com__EnemyHit.wav";

	SimpleMath::Vector3 m_nearEnemyPosition;

	//�R���C�_�[�i�[�ϐ�
	DirectX::BoundingBox m_collider;

	DirectXTK::CommonStates m_commonStates;

	SimpleMath::Matrix m_modelWorld;

	//�O�t���[���̃��[���h�s��
	SimpleMath::Matrix m_beforModelWorld;

	//���͎�t���̃J�����̍s��
	SimpleMath::Matrix m_moveStartcameraMatrix;

	SimpleMath::Vector3 m_modelVector;

	DirectXTK::Model m_model;

	Model::EffectCollection m_modelNomal;

	DirectXTK::EffectTextureFactory m_modelResources;

	DirectXTK::EffectFactory m_effectFactory;

	ModelBone::TransformArray m_draw_bones;

	ModelBone::TransformArray m_animation_bones;

	DX::AnimationSDKMESH m_animation[s_stateCount];

	//SE
	DirectXTK::Sound m_hitSe;

	PlayerMoveStruct m_playerMoveStruct;

	//����N���X
	Spear m_spear;
	Axe m_axe;

	StateOrigin* m_currentState;
	MoveState m_moveState;
	AttackState m_attackState;

	unsigned int m_playerStateIndex;

	float s_hitCoolCurrentTIme;

	bool m_moveStart;

	//�f�o�b�N---------------------------------------------------

	 //�f�o�b�N�� true �ɂ���ϐ�
	static constexpr bool s_DebugOn = true;
	// 
	//�f�o�b�O�p���f���֐�
	void CreateDebugModel();

	//�f�o�b�O�p���f���`��֐�
	void RenderDebugModel(DirectXTK::Camera&);

	std::unique_ptr<GeometricPrimitive> m_debugModel;
	std::unique_ptr<DirectX::BasicEffect> m_debugEffect;
};

