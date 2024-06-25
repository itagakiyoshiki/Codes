#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "EnemyAxe.h"

using namespace SimpleMath;

void EnemyAxe::LoadAssets(ResourceUploadBatch& resourceUploadBatch)
{

}

void EnemyAxe::Initialize(Enemy& enemy)
{
	m_parentMatrix = enemy.GetWorldMatrix();

	m_localScaleMatrix = Matrix::CreateScale(0.001f);
	m_localRotateMatrix = Matrix::Identity;
	m_localPositionMatrix = Matrix::Identity;

}

void EnemyAxe::OnDeviceLost()
{

}

void EnemyAxe::Update(Enemy& enemy)
{
	m_parentMatrix = enemy.GetWorldMatrix();


}

void EnemyAxe::Render(DirectXTK::Camera& camera, Enemy& enemy)
{
	m_worldMatrix = Matrix::Identity;
	Matrix _handMatrix = enemy.GetRightHandMatrix();

	m_worldMatrix = _handMatrix;

}

//void EnemyAxe::Apply(
//	const DirectX::Model& model,
//	size_t nbones,
//	XMMATRIX* boneTransforms) const
//{
//	assert(m_animData && m_animSize > 0);
//
//	if (!nbones || !boneTransforms)
//	{
//		throw std::invalid_argument("Bone transforms array required");
//	}
//
//	if (nbones < model.bones.size())
//	{
//		throw std::invalid_argument("Bone transforms array is too small");
//	}
//
//	if (model.bones.empty())
//	{
//		throw std::runtime_error("Model is missing bones");
//	}
//
//	auto header = reinterpret_cast<const SDKANIMATION_FILE_HEADER*>(m_animData.get());
//	assert(header->Version == SDKMESH_FILE_VERSION);
//
//	// Determine animation time
//	auto tick = static_cast<uint32_t>(static_cast<double>(header->AnimationFPS) * m_animTime);
//	tick %= header->NumAnimationKeys;
//
//	// Compute local bone transforms
//	auto frameData = reinterpret_cast<SDKANIMATION_FRAME_DATA*>(m_animData.get() + header->AnimationDataOffset);
//
//	for (size_t j = 0; j < nbones; ++j)
//	{
//		if (m_boneToTrack[j] == ModelBone::c_Invalid)
//		{
//			m_animBones[j] = model.boneMatrices[j];
//		}
//		else
//		{
//			auto frame = &frameData[m_boneToTrack[j]];
//			auto data = &frame->pAnimationData[tick];
//
//			XMVECTOR quat = XMVectorSet(data->Orientation.x, data->Orientation.y, data->Orientation.z, data->Orientation.w);
//			if (XMVector4Equal(quat, g_XMZero))
//				quat = XMQuaternionIdentity();
//			else
//				quat = XMQuaternionNormalize(quat);
//
//			XMMATRIX trans = XMMatrixTranslation(data->Translation.x, data->Translation.y, data->Translation.z);
//			XMMATRIX rotation = XMMatrixRotationQuaternion(quat);
//			XMMATRIX scale = XMMatrixScaling(data->Scaling.x, data->Scaling.y, data->Scaling.z);
//
//			m_animBones[j] = XMMatrixMultiply(XMMatrixMultiply(rotation, scale), trans);
//		}
//	}
//
//	// Compute absolute locations
//	model.CopyAbsoluteBoneTransforms(nbones, m_animBones.get(), boneTransforms);
//
//	// Adjust for model's bind pose.
//	for (size_t j = 0; j < nbones; ++j)
//	{
//		boneTransforms[j] = XMMatrixMultiply(model.invBindPoseMatrices[j], boneTransforms[j]);
//	}
//}
