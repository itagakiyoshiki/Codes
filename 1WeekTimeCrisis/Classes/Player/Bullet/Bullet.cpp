#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Bullet.h"
#include <iostream>
#include <vector>
#include <numeric>

using namespace SimpleMath;

void Bullet::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
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
}

void Bullet::Initialize()
{
	m_position = s_defaultPosition;

	m_rotation = Vector3(DXTK->Screen.Width / 2, DXTK->Screen.Height / 2, 0);

	m_oldRotation = m_rotation;

	m_modelWorld = Matrix::CreateWorld(
		m_position,
		m_modelWorld.Forward(),
		m_modelWorld.Up());

	//射影座標系からスクリーン座標系へ変換
	m_viewPortMatrix._11 = DXTK->Screen.Width / 2;
	m_viewPortMatrix._12 = 0;
	m_viewPortMatrix._13 = 0;
	m_viewPortMatrix._14 = 0;

	m_viewPortMatrix._21 = 0;
	m_viewPortMatrix._22 = -DXTK->Screen.Height / 2;
	m_viewPortMatrix._23 = 0;
	m_viewPortMatrix._24 = 0;

	m_viewPortMatrix._31 = 0;
	m_viewPortMatrix._32 = 0;
	m_viewPortMatrix._33 = 1;
	m_viewPortMatrix._34 = 0;

	m_viewPortMatrix._41 = DXTK->Screen.Width / 2;
	m_viewPortMatrix._42 = DXTK->Screen.Height / 2;
	m_viewPortMatrix._43 = 0;
	m_viewPortMatrix._44 = 1;

	m_rePopCurrentTime = 0.0f;

	m_fireOn = false;

	m_shotSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_shotSeFilePass);
}

void Bullet::Fire(DirectXTK::Camera camera)
{
	if (m_fireOn)
	{
		return;
	}

	m_fireOn = true;

	m_shotSe->Play();

	XMMATRIX _reverseViewPortMatrix = XMMatrixInverse(nullptr, m_viewPortMatrix);

	Vector2 mouse;
	mouse.x = m_rotation.x;
	mouse.y = m_rotation.y;

	m_viewPositon = Vector3(mouse.x, mouse.y, 0.0f);

	m_viewPositon = XMVector3Transform(m_viewPositon, _reverseViewPortMatrix);

	//m_targetPositon *=逆射影行列 ;
	XMMATRIX _reverseProjectionMatrix = XMMatrixInverse(nullptr, camera.GetProjectionMatrix());
	m_viewPositon = XMVector3Transform(m_viewPositon, _reverseProjectionMatrix);

	//m_targetPositon *= 逆ビュー行列 ;
	XMMATRIX _reverseViewMatrix = XMMatrixInverse(nullptr, camera.GetViewMatrix());
	m_viewPositon = XMVector3Transform(m_viewPositon, _reverseViewMatrix);

	m_position = camera.Position;

	Vector3 _position = m_position;

	Vector3 _normal = Vector3::Up;

	m_bulletZdir = m_viewPositon - _position;
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

void Bullet::Update(DirectXTK::Camera camera)
{
	m_rotation.x = m_oldRotation.x + InputSystem.Mouse.delta.x;
	m_rotation.y = m_oldRotation.y + InputSystem.Mouse.delta.y;

	m_oldRotation = m_rotation;

	if (m_fireOn)
	{
		m_position += (m_bulletRotation)*DXTK->Time.deltaTime * s_speed;
		m_rePopCurrentTime += DXTK->Time.deltaTime;

		if (m_rePopCurrentTime >= s_rePopTime)
		{
			FireOff(camera);
			return;
		}

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

	m_position = camera.Position;

	m_modelWorld = camera.ViewMatrix;
}

void Bullet::Hit(DirectXTK::Camera& camera)
{
	FireOff(camera);
}

void Bullet::FireOff(DirectXTK::Camera& camera)
{
	m_rePopCurrentTime = 0.0f;
	m_position = camera.Position;
	m_fireOn = false;
}

/// <summary>
/// 二つの座標の外積を返す関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Bullet::Cross(Vector3 v1, Vector3 v2) {
	return Vector3((v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

void Bullet::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Bullet::Render(DirectXTK::Camera& camera)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}