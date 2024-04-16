#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Stage.h"

using namespace SimpleMath;

void Stage::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	ModelLoad(resourceUploadBatch);
}

void Stage::ModelLoad(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_ModelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Models/Road");

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

void Stage::Initialize()
{
	//ˆÊ’u‚Ì‰Šú‰»
	m_position = Vector3(0, 0, 0);

	Vector3 _position = m_position;

	Vector3 _yVec = Vector3::Up;

	Vector3 _zVec = Vector3::Forward;

	Vector3 _xVec = Vector3::Right;

	Matrix _waldMat = Matrix::Identity;

	_waldMat._11 = _xVec.x; _waldMat._21 = _yVec.x; _waldMat._31 = _zVec.x;
	_waldMat._12 = _xVec.y; _waldMat._22 = _yVec.y; _waldMat._32 = _zVec.y;
	_waldMat._13 = _xVec.z; _waldMat._23 = _yVec.z; _waldMat._33 = _zVec.z;
	_waldMat._41 = m_position.x; _waldMat._42 = m_position.y; _waldMat._43 = m_position.z;

	m_modelWorld = _waldMat;
}

void Stage::Render(DirectXTK::Camera& camera)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);


	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}

void Stage::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}
