//
// TemplateScene.cpp
//

#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "../MainProject/Scenes/SceneFactory.h"

// Initialize member variables.
ResultScene::ResultScene()
{

}

// Start is called after the scene is created.
void ResultScene::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void ResultScene::LoadAssets()
{
	auto&& _device = DXTK->Device;

	m_resourceDescriptors = std::make_unique<DescriptorHeap>(_device, m_descriptors.Count);

	ResourceUploadBatch _resourceUpload(_device);

	_resourceUpload.Begin();

	m_bg.Load(_resourceUpload, m_resourceDescriptors, m_descriptors.ResultBG);

	m_retryButton.Load(_resourceUpload, m_resourceDescriptors);

	RenderTargetState _rtState(DXTK->BackBufferFormat,
		DXTK->DepthBufferFormat);

	SpriteBatchPipelineStateDescription _pd(_rtState
		, &CommonStates::NonPremultiplied);

	m_spriteBatch = std::make_unique<SpriteBatch>(_device, _resourceUpload, _pd);

	auto _viewport = DXTK->Screen.Viewport;
	m_spriteBatch->SetViewport(_viewport);

	auto _uploadResourcesFinished = _resourceUpload.End(
		DXTK->CommandQueue);

	_uploadResourcesFinished.wait();
}

// Initialize a variable and audio resources.
void ResultScene::Initialize()
{
	m_retryButton.Initialize();
	m_loadScene = LoadScene::Stay;

	//WinLose画像の切り替え実装予定地
	/*
	if(DontDestroy->m_player1Win)
	{

	}
	else
	{

	}
	*/
}

// Releasing resources required for termination.
void ResultScene::Terminate()
{
	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void ResultScene::OnDeviceLost()
{
	m_bg.Reset();
	m_retryButton.Reset();
}

// Restart any looped sounds here
void ResultScene::OnRestartSound()
{

}

// Updates the scene.
NextScene ResultScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	if (InputSystem.Keyboard.wasPressedThisFrame.Space
		&& m_loadScene == LoadScene::Stay)
	{
		m_loadScene = LoadScene::Title;
		return NextScene::TitleScene;
	}

	if (InputSystem.Keyboard.wasPressedThisFrame.Enter
		&& m_loadScene == LoadScene::Stay)
	{
		m_retryButton.AnimationStart();
		m_loadScene = LoadScene::Result;



		return NextScene::MainScene;//==========今はすぐにシーン遷移を行う



	}

	//ボタンアニメーション終わったらシーン遷移を実行させる
	if (m_loadScene == LoadScene::Result)
	{
		m_retryButton.AnimationUpdate();

		if (m_retryButton.AnimationEnd())
		{
			return NextScene::MainScene;
		}
	}

	return NextScene::Continue;
}

// Draws the scene.
void ResultScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);

	m_spriteBatch->Begin(DXTK->CommandList);

	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

	auto _spriteBatch = m_spriteBatch.get();

	m_bg.Draw(_spriteBatch);

	m_retryButton.Draw(_spriteBatch);

	m_spriteBatch->End();


	DXTK->ExecuteCommandList();
}
