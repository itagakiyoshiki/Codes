#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Stage.h"

void Stage::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromCMO(
		device, s_modelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch, L"Model");

	m_effectFactory = DirectXTK::CreateEffectFactory(
		m_modelResources->Heap(), m_commonStates->Heap());

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullCounterClockwise,
		rtState
	);

	m_modelNomal = m_model->CreateEffects(*m_effectFactory.get(), pd, pd);
}

void Stage::Initialize()
{
	m_modelWorld = SimpleMath::Matrix::Identity;
}

void Stage::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Stage::Render(DirectXTK::Camera camera)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}