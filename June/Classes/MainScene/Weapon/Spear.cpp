#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Spear.h"

using namespace SimpleMath;

void Spear::LoadAssets(ResourceUploadBatch& resourceUploadBatch, const bool playerUse)
{
	m_playerUse = playerUse;

	auto&& device = DXTK->Device;

	m_commonStates = std::make_unique<CommonStates>(device);

	m_effectFactory = std::make_unique<EffectFactory>(device);

	if (m_playerUse)
	{
		m_model = DirectX::Model::CreateFromSDKMESH(
			device, s_playerVerModelFilePass);

		m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
			L"Model/Weapon");
	}
	else
	{
		m_model = DirectX::Model::CreateFromSDKMESH(
			device, s_enemyVerModelFilePass);

		m_modelResources = m_model->LoadTextures(device, resourceUploadBatch,
			L"Model/Weapon/Enemy");
	}


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

	EffectLoad();

	CreateDebugModel(rtState);
}

void Spear::Initialize()
{

	m_collider.Center = Vector3::Zero;
	m_collider.Extents = s_colliderOffSize;

	m_position = Vector3::Zero;
}

void Spear::OnDeviceLost()
{
	m_modelNomal.clear();

	m_effectFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_commonStates.reset();
}

void Spear::Render(DirectXTK::Camera& camera)
{
	RenderDebugModel(camera);

	ModelMatrixUpdate();

	EffectUpdate(camera);


	ID3D12DescriptorHeap* heaps[]{ m_modelResources->Heap(),m_commonStates->Heap() };

	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	Model::UpdateEffectMatrices(
		m_modelNomal, m_modelWorld,
		camera.ViewMatrix, camera.ProjectionMatrix
	);

	m_model->Draw(DXTK->CommandList, m_modelNomal.cbegin());
}

/// <summary>
/// ポジションをセットし、コライダーを更新します
/// </summary>
/// <param name="position"></param>
void Spear::SetPosition(const SimpleMath::Vector3 position)
{
	m_position = position;
	ColliderUpdate();
}

/// <summary>
/// 穂先を向ける向きを指定します
/// </summary>
/// <param name="position"></param>
void Spear::SetTargetPosition(const SimpleMath::Vector3 position)
{
	m_targetVector = m_position - position;
	m_targetVector.y = 0.0f;
	m_targetVector.Normalize();

	m_targetPosition = position + m_targetVector * s_targetVectorOffset;

	ModelMatrixUpdate();

	ColliderUpdate();
}

/// <summary>
/// エフェクトを読み込み初期設定も行う
/// </summary>
void Spear::EffectLoad()
{
	//Effect
	DXGI_FORMAT _renderTargetFormats = DXTK->BackBufferFormat;
	m_efkRenderer = EffekseerRendererDX12::Create(
		DXTK->Device,
		DXTK->CommandQueue,
		2,
		&_renderTargetFormats,
		1,
		DXTK->DepthBufferFormat,
		false,
		10000
	);

	m_efkManager = Effekseer::Manager::Create(10000);//最大インスタンス数
	//座標系を左手系にする
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	//描画用インスタンスから描画機能を設定
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	//描画用インスタンスからテクスチャの読み込み機能を設定
	//独自拡張も可能
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());

	//DirectX12特有の処理
	m_efkMemoryPool =
		EffekseerRenderer::CreateSingleFrameMemoryPool(m_efkRenderer->GetGraphicsDevice());

	m_efkCmdList = EffekseerRenderer::CreateCommandList(
		m_efkRenderer->GetGraphicsDevice(), m_efkMemoryPool);

	m_efkRenderer->SetCommandList(m_efkCmdList);

	m_effect = Effekseer::Effect::Create(
		m_efkManager,
		(const EFK_CHAR*)L"Effect/JuneSpearEffect.efk",
		1.0f,
		(const EFK_CHAR*)L"Effect/");
}

/// <summary>
/// エフェクトの描画を更新する
/// </summary>
/// <param name="camera"></param>
void Spear::EffectUpdate(DirectXTK::Camera& camera)
{
	m_efkManager->Update();//マネージャーの時間更新
	m_efkMemoryPool->NewFrame();//描画すべきレンダーターゲットを選択

	EffekseerRendererDX12::BeginCommandList(m_efkCmdList, DXTK->CommandList);

	m_efkRenderer->BeginRendering();//描画前処理
	m_efkManager->Draw();//エフェクト描画
	m_efkRenderer->EndRendering();//描画後処理

	EffekseerRendererDX12::EndCommandList(m_efkCmdList);

	Effekseer::Matrix44 _fkViewMat;
	Effekseer::Matrix44 _fkProjMat;
	auto _view = camera.ViewMatrix;
	auto _proj = camera.ProjectionMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_fkViewMat.Values[i][j] = _view.r[i].m128_f32[j];
			_fkProjMat.Values[i][j] = _proj.r[i].m128_f32[j];
		}
	}

	m_efkRenderer->SetCameraMatrix(_fkViewMat);
	m_efkRenderer->SetProjectionMatrix(_fkProjMat);
}

/// <summary>
/// コライダーに当たった時に呼ばれる関数
/// </summary>
void Spear::OnCollisionEnter()
{
	m_efkHandle = m_efkManager->Play(m_effect, m_position.x, m_position.y, m_position.z);
	m_efkManager->SetScale(m_efkHandle, s_effectScale, s_effectScale, s_effectScale);
}

/// <summary>
/// コライダーの位置情報を更新
/// </summary>
void Spear::ColliderUpdate()
{
	m_colliderCenterPosition = m_position;

	Vector3 _forwordVector = m_modelWorld.Up();

	m_colliderCenterPosition = m_colliderCenterPosition + _forwordVector * 100.0f;

	m_collider.Center = m_colliderCenterPosition;
}

/// <summary>
///	モデルの行列を更新
/// </summary>
void Spear::ModelMatrixUpdate()
{
	m_modelWorld = Matrix::CreateScale(s_defaultScale);

	m_modelWorld *= ModelLookAt(m_targetPosition, m_position);

	m_modelWorld._41 = m_position.x;
	m_modelWorld._42 = m_position.y;
	m_modelWorld._43 = m_position.z;
}

/// <summary>
/// 渡された行列をtargetへ向けた行列を返す関数 
/// </summary>
/// <param name="target"></param>
Matrix Spear::ModelLookAt(Vector3 target, Vector3 position)
{
	Vector3 _targetPosition = target;
	_targetPosition.y = 0.0f;
	Vector3 _myPosition = position;
	_myPosition.y = 0.0f;

	Vector3 _y = (_targetPosition - _myPosition);
	_y.Normalize();

	Vector3 _x = ItagakiMath::Cross(Vector3::Up, _y);
	_x.Normalize();

	Vector3 _z = ItagakiMath::Cross(_y, _x);
	_z.Normalize();


	Matrix _worldMatrix = Matrix::Identity;

	_worldMatrix._11 = _x.x; _worldMatrix._21 = _y.x; _worldMatrix._31 = _z.x;
	_worldMatrix._12 = _x.y; _worldMatrix._22 = _y.y; _worldMatrix._32 = _z.y;
	_worldMatrix._13 = _x.z; _worldMatrix._23 = _y.z; _worldMatrix._33 = _z.z;

	return _worldMatrix;
}


/// <summary>
/// コライダー確認用モデルを作成
/// </summary>
/// <param name="_rtState"></param>
void Spear::CreateDebugModel(RenderTargetState _rtState)
{
	if (s_DebugOn)
	{
		EffectPipelineStateDescription _debug_pd(
			&GeometricPrimitive::VertexType::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthDefault,
			CommonStates::CullNone,
			_rtState);

		m_debugModel = DirectX::GeometricPrimitive::CreateBox(s_colliderOnSize / 2);
		m_debugEffect = std::make_unique<BasicEffect>(DXTK->Device, EffectFlags::Lighting, _debug_pd);
		m_debugEffect->EnableDefaultLighting();
	}
}

/// <summary>
/// コライダー確認用モデルを描画
/// </summary>
/// <param name="_view"></param>
/// <param name="_proj"></param>
void Spear::RenderDebugModel(DirectXTK::Camera& camera)
{
	if (s_DebugOn)
	{
		Matrix _worldmatrix = Matrix::Identity;
		_worldmatrix *= Matrix::CreateScale(1.0f);
		_worldmatrix *= Matrix::CreateWorld(
			m_colliderCenterPosition, Vector3::Forward, Vector3::Up
		);

		_worldmatrix._41 = m_colliderCenterPosition.x;
		_worldmatrix._42 = m_colliderCenterPosition.y;
		_worldmatrix._43 = m_colliderCenterPosition.z;

		m_debugEffect->SetWorld(_worldmatrix);
		m_debugEffect->SetView(camera.ViewMatrix);
		m_debugEffect->SetProjection(camera.ProjectionMatrix);
		m_debugEffect->Apply(DXTK->CommandList);
		m_debugModel->Draw(DXTK->CommandList);
	}
}

