#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "CounterManager.h"

using namespace SimpleMath;

void CounterManager::Load(ResourceUploadBatch& esourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_sprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_spriteFilePass,
		esourceUpload,
		resourceDescriptors,
		static_cast<int>(DescriptorStorage::Descriptors::CounterSprite));
}

void CounterManager::Initialize()
{
	m_playerPosition = Vector3::Zero;

	m_spritePosition = Vector2(DXTK->Screen.Width / 2, DXTK->Screen.Height / 2);

	m_spriteOldPosition = m_spritePosition;

	m_counterOn = false;

	m_spriteOn = false;
}

void CounterManager::Update(Enemy& enemy, DirectXTK::Camera camera)
{
	//counter�\�͈͂Ȃ�΃t���O�𗧂Ă�
	bool _counterDistanceIn = false;
	if (ItagakiMath::Distance(enemy.GetPosition(), m_playerPosition) <= s_counterDistance)
	{
		_counterDistanceIn = true;
	}

	//�J�E���^�[��t��ԂɓG���Ȃ�����摜�\��
	if (enemy.GetAttackState().GetCounterOk() && _counterDistanceIn)
	{
		m_spriteOn = true;

		//�G���[���h���W���X�N���[�����W�ɕϊ�����
		m_spritePosition = WorldToScreenPos(enemy.GetPosition(), camera);

		m_spritePosition.x += -(m_sprite.size.x / s_spriteOffsetX);
		m_spritePosition.y += -(m_sprite.size.y / s_spriteOffsetY);

		m_spriteOldPosition = m_spritePosition;

	}
	else
	{
		m_spriteOn = false;
	}


	//OnCounter()���Ă΂�ĂȂ���ΏI��
	//�G���J�E���^�[��t�Ȃ���ΏI��
	if (!enemy.GetAttackState().GetCounterOk() && m_counterOn)
	{
		m_counterOn = false;
		return;
	}

	//counter�\�͈͂Ȃ�Ώ������s��
	if (m_counterOn && _counterDistanceIn)
	{
		m_spriteOn = false;
		enemy.GetAttackState().OnCounter(enemy);
	}

}

void CounterManager::OnDeviceLost()
{
	m_sprite.resource.Reset();
}

void CounterManager::Draw(SpriteBatch* batch)
{

	if (m_spriteOn)
	{
		batch->Draw(
			m_sprite.handle, m_sprite.size, m_spritePosition, nullptr,
			Colors::White, s_drawRotation, g_XMZero, s_drawScale, DirectX::DX12::SpriteEffects_None);
	}

}

void CounterManager::OnCounter(const Vector3& playerPosition)
{
	m_playerPosition = playerPosition;

	m_counterOn = true;
}

/// <summary>
/// ���[���h���W���X�N���[�����W�ɕϊ���������Ԃ��܂�
/// </summary>
/// <param name="worldPos"></param>
/// <returns></returns>
Vector2 CounterManager::WorldToScreenPos(Vector3 worldPos, DirectXTK::Camera camera)
{
	float _screenWidth = DXTK->Screen.Width / 2;
	float _screenHeight = DXTK->Screen.Height / 2;

	Matrix _viewPort =
	{
		_screenWidth,0,0,0,
		0,-_screenHeight,0,0,
		0,0,1,0,
		_screenWidth,_screenHeight,0,1
	};

	Vector3 _worldVector = XMVector3Transform(worldPos, camera.ViewMatrix);
	_worldVector = XMVector3Transform(_worldVector, camera.ProjectionMatrix);

	_worldVector = XMVectorSet(_worldVector.x / _worldVector.z, _worldVector.y / _worldVector.z, 1.0f, 1.0f);

	Vector2 _retunVector = XMVector3Transform(_worldVector, _viewPort);

	return _retunVector;
}

