#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Stage.h"

using namespace SimpleMath;

void Stage::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_modelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Model/Stage/Road");

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

void Stage::Initialize(Vector3 position)
{
	m_modelWorld *= Matrix::CreateScale(s_defaultScale);
	m_modelWorld *= Matrix::CreateRotationZ(s_defaultRotationZ * Mathf::Deg2Rad);

	m_position = position;

	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;
}

void Stage::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
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

