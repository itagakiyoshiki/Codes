#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "PlayerModel.h"

using namespace SimpleMath;

void PlayerModel::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{
	ModelLoad(resourceUploadBatch);
}

void PlayerModel::ModelLoad(ResourceUploadBatch& resourceUploadBatch)
{
	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	m_model = DirectX::Model::CreateFromSDKMESH(
		device, s_ModelFilePass);

	m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
		L"Models/Player/Player");

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

void PlayerModel::Render(DirectXTK::Camera& camera, SimpleMath::Matrix modelMatrix)
{
	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, modelMatrix,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}

void PlayerModel::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}


