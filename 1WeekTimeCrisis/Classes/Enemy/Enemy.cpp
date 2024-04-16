#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Enemy.h"

using namespace SimpleMath;

void Enemy::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromCMO(
		device, s_modelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Model");

	m_effectFactory = DirectXTK::CreateEffectFactory(
		m_modelResources->Heap(), m_commonStates->Heap());

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState
	);

	m_modelNomal = m_model->CreateEffects(*m_effectFactory.get(), pd, pd);

	m_enemyBullet.LoadAssets(resourceUploadBatch);

}

void Enemy::Initialize()
{
	m_popIndex = 0;

	m_shootCoolTimeIndex = 0;

	m_shootCooltime = s_shootCoolTimeArray[m_shootCoolTimeIndex];

	m_position = s_popPosition[m_popIndex];

	m_rotation = s_defaultRotation;

	m_modelWorld = Matrix::CreateWorld(
		m_position,
		m_modelWorld.Forward(),
		m_modelWorld.Up());

	m_enemyBullet.Initialize();

	m_shootCoolCurrentTime = 0.0f;

	m_isDeath = false;

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);
}

void Enemy::Update(DirectXTK::Camera camera, bool playerIsDeath)
{
	Vector3 _position = m_position;

	Vector3 _targetPosition = camera.GetPosition();

	Vector3 _targetDis = _targetPosition - _position;

	Vector3 _normal = Vector3::Up;

	Vector3 _xCross = Cross(_normal, _targetDis);
	_xCross.Normalize();

	Vector3 _yZiku = Cross(_targetDis, _xCross);
	_yZiku.Normalize();

	//ワールド行列をつくる
	Matrix _waldMat = Matrix::Identity;

	_waldMat._11 = _xCross.x; _waldMat._21 = _yZiku.x; _waldMat._31 = _targetDis.x;
	_waldMat._12 = _xCross.y; _waldMat._22 = _yZiku.y; _waldMat._32 = _targetDis.y;
	_waldMat._13 = _xCross.z; _waldMat._23 = _yZiku.z; _waldMat._33 = _targetDis.z;
	_waldMat._41 = m_position.x; _waldMat._42 = m_position.y; _waldMat._43 = m_position.z;

	m_modelWorld = _waldMat;

	//射影行列とビュー行列を作る
	XMMATRIX viewMatrix = Matrix::Identity;
	viewMatrix = Matrix::CreateWorld(m_position, s_outPosiiton, Vector3::Up);

	SimpleMath::Vector3 _vec = s_outPosiiton - m_position;
	_vec.Normalize();

	XMMATRIX proMatrix = DirectX::XMMatrixLookToLH(m_position, _vec, Vector3::Up);

	m_shootCoolCurrentTime += DXTK->Time.deltaTime;
	if (!m_enemyBullet.GetFireOn()
		&& m_shootCoolCurrentTime >= m_shootCooltime
		&& !playerIsDeath
		&& !m_isDeath)
	{
		m_enemyBullet.Fire(s_outPosiiton);
		m_shootCoolCurrentTime = 0.0f;

		//クールタイムを更新
		m_shootCoolTimeIndex++;
		if (m_shootCoolTimeIndex >= s_shootCoolTimeArrayCount)
		{
			m_shootCoolTimeIndex = 0;
		}

		m_shootCooltime = s_shootCoolTimeArray[m_shootCoolTimeIndex];
	}

	m_enemyBullet.Update(proMatrix, viewMatrix, m_position);
}

Vector3 Enemy::Cross(Vector3 v1, Vector3 v2) {
	return Vector3((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

void Enemy::Hit()
{
	m_popIndex++;

	m_hitSe->Play();

	//規定回数当てたら天高く昇って画面から消える
	if (m_popIndex > s_clearLineCount)
	{
		//s_popPosition配列の最後の要素はゲームクリア用の要素
		m_popIndex = s_popPositionCount - 1;
		m_isDeath = true;
	}
	m_position = s_popPosition[m_popIndex];

	m_enemyBullet.Fire(s_outPosiiton);
}

void Enemy::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();

	m_enemyBullet.OnDeviceLost();
}

void Enemy::Render(DirectXTK::Camera camera)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());

	m_enemyBullet.Render(camera);
}