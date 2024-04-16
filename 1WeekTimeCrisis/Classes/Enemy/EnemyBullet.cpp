#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyBullet.h"
#include <iostream>
#include <vector>
#include <numeric>

using namespace SimpleMath;

void EnemyBullet::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromCMO(
		device,
		L"Model/EnemyBullet1Week.cmo");

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
}

void EnemyBullet::Initialize()
{
	m_position = s_defaultPosition;

	m_rotation = s_defaultRotation;

	m_modelWorld = Matrix::CreateWorld(
		m_position,
		m_modelWorld.Forward(),
		m_modelWorld.Up());

	m_rePopCurrentTime = 0.0f;

	m_fireOn = false;

	m_shotSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_shotSeFilePass);
}

void EnemyBullet::Fire(Vector3 targetPosition)
{
	if (m_fireOn)
	{
		return;
	}

	m_shotSe->Play();

	m_fireOn = true;

	m_targetPositon = targetPosition;

	//カーソルが示す方向に向きを作る
	Vector3 _position = m_position;

	Vector3 _normal = Vector3::Up;

	m_bulletZdir = m_targetPositon - _position;
	m_bulletZdir.Normalize();

	Vector3 _xCross = Cross(_normal, m_bulletZdir);
	_xCross.Normalize();

	Vector3 _yZiku = Cross(m_bulletZdir, _xCross);
	_yZiku.Normalize();

	//ワールド行列をつくる
	Matrix _waldMat = Matrix::Identity;

	_waldMat._11 = _xCross.x; _waldMat._21 = _yZiku.x; _waldMat._31 = m_bulletZdir.x;
	_waldMat._12 = _xCross.y; _waldMat._22 = _yZiku.y; _waldMat._32 = m_bulletZdir.y;
	_waldMat._13 = _xCross.z; _waldMat._23 = _yZiku.z; _waldMat._33 = m_bulletZdir.z;
	_waldMat._41 = m_position.x; _waldMat._42 = m_position.y; _waldMat._43 = m_position.z;

	m_modelWorld = _waldMat;

	m_bulletRotation = m_bulletZdir;

	m_rePopCurrentTime = 0.0f;
}

void EnemyBullet::Update(DirectX::XMMATRIX proMat,
	DirectX::XMMATRIX viewMat,
	SimpleMath::Vector3 pos)
{
	if (m_fireOn)
	{
		m_position += (m_bulletRotation)*DXTK->Time.deltaTime * s_speed;
		m_rePopCurrentTime += DXTK->Time.deltaTime;

		//弾に当たらず一定時間たったら消える
		if (m_rePopCurrentTime >= s_rePopTime)
		{
			FireOff(pos);
			return;
		}

		//Fire()関数で決められた方向を向き続ける
		Vector3 _normal = Vector3::Up;

		Vector3 _xCross = Cross(_normal, m_bulletRotation);
		_xCross.Normalize();

		Vector3 _yZiku = Cross(m_bulletRotation, _xCross);
		_yZiku.Normalize();

		//ワールド行列をつくる
		Matrix _waldMat = Matrix::Identity;

		_waldMat._11 = _xCross.x; _waldMat._21 = _yZiku.x; _waldMat._31 = m_bulletRotation.x;
		_waldMat._12 = _xCross.y; _waldMat._22 = _yZiku.y; _waldMat._32 = m_bulletRotation.y;
		_waldMat._13 = _xCross.z; _waldMat._23 = _yZiku.z; _waldMat._33 = m_bulletRotation.z;
		_waldMat._41 = m_position.x; _waldMat._42 = m_position.y; _waldMat._43 = m_position.z;

		m_modelWorld = _waldMat;

		return;

	}

	m_position = pos;

	m_modelWorld = viewMat;


}

void EnemyBullet::FireOff(SimpleMath::Vector3 pos)
{
	m_rePopCurrentTime = 0.0f;
	m_position = pos;
	m_fireOn = false;
}

/// <summary>
/// 与えられた座標同士の外積を返す
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 EnemyBullet::Cross(Vector3 v1, Vector3 v2) {
	return Vector3((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

void EnemyBullet::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void EnemyBullet::Render(DirectXTK::Camera& camera)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}