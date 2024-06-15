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
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
	m_playerCamera.LoadAssets();

	auto&& device = DXTK->Device;

	m_resourceDescriptors =
		std::make_unique<DescriptorHeap>(device, static_cast<int>(DescriptorStorage::Descriptors::Count));

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();


	m_stage.LoadAssets(resourceUpload);

	m_player.LoadAssets(resourceUpload, m_playerCamera.GetCamera());

	m_enemy.LoadAssets(resourceUpload);

	m_counterManager.Load(resourceUpload, m_resourceDescriptors);

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
	m_stage.Initialize(s_stagePosition);

	m_player.Initialize();

	m_enemy.Initialize(s_enemyStartPosition);

	m_counterManager.Initialize();
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

	m_player.OnDeviceLost();

	m_enemy.OnDeviceLost();

	m_counterManager.OnDeviceLost();

	m_spriteBatch.reset();
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{

}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.

	m_player.Update(m_playerCamera.GetCamera(), m_counterManager, m_enemy);

	m_counterManager.Update(m_enemy, m_playerCamera.GetCamera());

	m_enemy.Update(m_player.GetPlayerMoveStruct().GetPosition(),
		m_playerCamera.GetCamera(),
		m_player.GetAttackState().GetSpearOn()
	);

	m_colliderManager.ColliderResolve(m_player, m_enemy);

	m_playerCamera.Update(m_player.GetWorldMatrix());

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);

	m_stage.Render(m_playerCamera.GetCamera());

	m_player.Render(m_playerCamera.GetCamera());

	m_enemy.Render(m_playerCamera.GetCamera());

	//‰æ‘œ‚Æƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý‚ð•ª‚¯‚Äs‚¤

	ID3D12DescriptorHeap* _heaps[] = { m_resourceDescriptors->Heap() };
	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(_heaps)), _heaps);

	auto _spriteBatch = m_spriteBatch.get();

	m_spriteBatch->Begin(DXTK->CommandList);

	m_counterManager.Draw(_spriteBatch);

	m_spriteBatch->End();

	DXTK->ExecuteCommandList();
}
