#pragma once
#include "Scenes/Scene.h"
#include "Classes/MainScene/Enemy/Enemy.h"

using namespace SimpleMath;

class EnemyAxe
{
public:
	void LoadAssets(ResourceUploadBatch& resourceUploadBatch);

	void Initialize(Enemy& enemy);

	void OnDeviceLost();

	void Update(Enemy& enemy);

	void Render(DirectXTK::Camera& camera, Enemy& enemy);

private:

	void Apply(
		const DirectX::Model& model,
		size_t nbones,
		XMMATRIX* boneTransforms)const;

	Matrix m_worldMatrix;
	Matrix m_parentMatrix;
	Matrix m_localScaleMatrix;
	Matrix m_localRotateMatrix;
	Matrix m_localPositionMatrix;

	//internal Matrix MMatrix = >
	//	(LocalScaleMatrix * LocalRotateMatrix * LocalPositionMatrix)
	//	* (_parent != null ? _parent.MMatrix : Matrix.Identity);
};

