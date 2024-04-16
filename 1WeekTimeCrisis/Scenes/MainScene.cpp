//
// MainScene.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"

using namespace SimpleMath;

// Initialize member variables.
MainScene::MainScene()
{

}

// Start is called after the scene is created.
void MainScene::Start()
{
	Initialize();
	LoadAssets();
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
	auto&& device = DXTK->Device;

	m_resourceDescriptors = std::make_unique<DescriptorHeap>(device, m_descriptors.Count);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	m_stage.LoadAssets(resourceUpload);

	m_player.LoadAssets(resourceUpload, m_resourceDescriptors);

	m_enemy.LoadAssets(resourceUpload);

	m_mainSceneText.Load(resourceUpload, m_resourceDescriptors);

	RenderTargetState _rtState(DXTK->BackBufferFormat,
		DXTK->DepthBufferFormat);

	SpriteBatchPipelineStateDescription pd(_rtState
		, &CommonStates::NonPremultiplied);

	m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

	auto _viewport = DXTK->Screen.Viewport;
	m_spriteBatch->SetViewport(_viewport);

	auto uploadResourcesFinished = resourceUpload.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();
}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	m_camera.SetView(s_defaultPosition, s_defaultRotation);

	m_camera.SetPerspectiveFieldOfView(
		s_viewAngle, (float)DXTK->Screen.Width / (float)DXTK->Screen.Height,
		s_nearz, s_farz);

	m_stage.Initialize();

	m_player.Initialize();

	m_enemy.Initialize();

	m_mainSceneText.Initialize();

	m_camera.SetPosition(m_player.GetPosition());

	InputSystem.Mouse.SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);

	m_mainBgm = DirectXTK::CreateSound(DXTK->AudioEngine, s_mainBGMFilePass);

	m_gameClearBgm = DirectXTK::CreateSound(DXTK->AudioEngine, s_gameClearBGMFilePass);

	m_gameOverBgm = DirectXTK::CreateSound(DXTK->AudioEngine, s_gameOverBGMFilePass);

	m_bgmInstance = m_mainBgm->CreateInstance();
	m_bgmInstance->Play();

	m_gameEnd = false;

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
	m_stage.OnDeviceLost();

	m_player.Reset();

	m_enemy.OnDeviceLost();

	m_spriteBatch.reset();

	m_mainSceneText.Reset();
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{

}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	m_player.Update(m_camera);

	//リトライ処理
	if (InputSystem.Keyboard.wasPressedThisFrame.F && m_player.GetDeath() ||
		InputSystem.Keyboard.wasPressedThisFrame.F && m_enemy.GetIsDeath())
	{
		return NextScene::MainScene;
	}


	m_enemy.Update(m_camera, m_player.GetDeath());

	//敵とプレイヤーの弾の距離が一定値以下なら当たったとする
	float _enemyBulletDistance = std::sqrt(
		pow(m_player.GetBulletPosition().x - m_enemy.GetPosition().x, 2) +
		pow(m_player.GetBulletPosition().y - m_enemy.GetPosition().y, 2));
	if (_enemyBulletDistance <= s_hitDistance)
	{
		m_player.BulletHit(m_camera);
		m_enemy.Hit();
	}

	//敵が死んだらゲームが終わったと判断する
	if (m_enemy.GetIsDeath() && !m_gameEnd)
	{
		m_mainSceneText.GameEnd(false);
		if (!m_gameEnd)
		{
			m_bgmInstance.reset();
			m_bgmInstance = m_gameClearBgm->CreateInstance();
			m_bgmInstance->Play();
		}
		m_gameEnd = true;
	}

	//プレイヤーが出ていて敵弾との距離が一定値以下なら当たったとする
	if (m_player.GetMovingState() == PlayerMove::MovingState::Out)
	{
		//敵とプレイヤーの弾の距離が一定値以下なら当たったとする
		float _enemyBulletToPlayerDistance = std::sqrt(
			pow(m_player.GetPosition().x - m_enemy.GetBulletPosition().x, 2) +
			pow(m_player.GetPosition().y - m_enemy.GetBulletPosition().y, 2)
		);

		//弾と当たったらゲームオーバー
		if (_enemyBulletToPlayerDistance <= s_hitDistance)
		{
			m_player.Death();
			m_mainSceneText.GameEnd(true);

			m_bgmInstance.reset();
			m_bgmInstance = m_gameOverBgm->CreateInstance();
			m_bgmInstance->Play();
		}
	}

	//カメラの位置角度更新
	m_camera.SetView(
		m_player.GetDefaultViewVector(),
		m_player.GetViewVector());

	m_camera.SetPosition(m_player.GetPosition());

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);

	m_stage.Render(m_camera);

	m_player.ModelRender(m_camera);

	m_enemy.Render(m_camera);

	//画像とモデルの読み込みを分けて行う

	ID3D12DescriptorHeap* _heaps[] = { m_resourceDescriptors->Heap() };
	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(_heaps)), _heaps);

	auto _spriteBatch = m_spriteBatch.get();

	m_spriteBatch->Begin(DXTK->CommandList);

	m_player.SpriteRender(_spriteBatch);

	m_mainSceneText.Draw(_spriteBatch);

	m_spriteBatch->End();

	DXTK->ExecuteCommandList();
}
