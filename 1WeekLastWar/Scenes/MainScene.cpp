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
	auto&& device = DXTK->Device;

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	m_player.LoadAssets(resourceUpload);

	m_stage.LoadAssets(resourceUpload);

	m_enemyManeger.LoadAssets(resourceUpload);

	m_itemManeger.LoadAssets(resourceUpload);

	auto uploadResourcesFinished = resourceUpload.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();
}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	m_camera.SetView(
		Vector3(0.0f, 10.6f, -17.0f),
		Vector3(90.0f, 0.0f, 0.0f)
	);

	m_camera.SetPerspectiveFieldOfView(
		Mathf::PI / 4.0f,
		(float)DXTK->Screen.Width / (float)DXTK->Screen.Height,
		0.1f, 10000.0f);


	m_player.Initialize();

	m_stage.Initialize();

	m_enemyManeger.Initialize();

	m_itemManeger.Initialize();

	m_viewPositon = Vector3(0, 0.1f, -1);
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
	m_player.OnDeviceLost();

	m_stage.OnDeviceLost();

	m_enemyManeger.OnDeviceLost();

	m_itemManeger.OnDeviceLost();
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

	m_hitHudgment.BulletEnemyIntercect(m_player.GetBulletSturct(), m_enemyManeger.GetEnemySturct());

	m_hitHudgment.PlayerItemIntercect(m_player, m_itemManeger.GetItemStruct());

	m_player.Update();

	m_enemyManeger.Update();

	m_itemManeger.Update();

	Vector3 _nowPos = m_camera.GetPosition();

	m_camera.SetViewLookAt(_nowPos, m_viewPositon, Vector3::Up);

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);

	m_player.Render(m_camera);

	m_stage.Render(m_camera);

	m_enemyManeger.Render(m_camera);

	m_itemManeger.Render(m_camera);

	DXTK->ExecuteCommandList();
}
