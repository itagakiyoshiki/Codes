//
// MainScene.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"


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
	auto&& _device = DXTK->Device;

	m_resourceDescriptors = std::make_unique<DescriptorHeap>(_device, m_descriptors.Count);

	ResourceUploadBatch _resourceUpload(_device);

	_resourceUpload.Begin();

	m_bg.Load(_resourceUpload, m_resourceDescriptors, m_descriptors.BG);

	m_bookObject.Load(_resourceUpload, m_resourceDescriptors);

	m_itemObject.Load(_resourceUpload, m_resourceDescriptors);

	//===========================================================================Player_1
	m_player_1.Load(_resourceUpload, m_resourceDescriptors, m_descriptors.Player_1);
	//===========================================================================
	//===========================================================================Player_2
	m_player_2.Load(_resourceUpload, m_resourceDescriptors, m_descriptors.Player_2);
	//===========================================================================

	RenderTargetState _rtState(DXTK->BackBufferFormat,
		DXTK->DepthBufferFormat);

	SpriteBatchPipelineStateDescription pd(_rtState
		, &CommonStates::NonPremultiplied);

	m_spriteBatch = std::make_unique<SpriteBatch>(_device, _resourceUpload, pd);

	auto _viewport = DXTK->Screen.Viewport;
	m_spriteBatch->SetViewport(_viewport);

	auto _uploadResourcesFinished = _resourceUpload.End(
		DXTK->CommandQueue);

	m_mainSceneShader.LoadAssets();
}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	//===========================================================================Player_1
	m_player_1.Initialize(m_descriptors.Player_1);
	//===========================================================================
	//===========================================================================Player_2
	m_player_2.Initialize(m_descriptors.Player_2);
	//===========================================================================

	m_bookObject.Initialize();

	m_itemObject.Initialize();

	m_mainSceneShader.Initialize();

	m_hitSe = DirectXTK::CreateSound(DXTK->AudioEngine, s_hitSeFilePass);

	m_mainToResultWipeSize = 0.0f;

	m_changeSceneCurrentTime = 0.0f;

	m_changeScene = false;

	m_gameEnd = false;

	DontDestroy->m_player1Win = false;

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
	/*m_bgSprite.resource.Reset();*/
	m_bg.Reset();
	m_bookObject.Reset();
	//===========================================================================Player_1
	m_player_1.Reset();
	//===========================================================================
	//===========================================================================Player_2
	m_player_2.Reset();
	//===========================================================================
	m_mainSceneShader.Terminate();
	m_resourceDescriptors.reset();
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

	//�Q�[�����I������ꍇ����ȍ~�̏����͍s���Ȃ�
	if (m_gameEnd)
	{
		m_mainToResultWipeSize += 400.0f * DXTK->Time.deltaTime;

		m_changeSceneCurrentTime += DXTK->Time.deltaTime;

		//�f�o�b�O�p�V�[���J��
		if (InputSystem.Keyboard.wasPressedThisFrame.Space
			|| m_changeSceneCurrentTime >= s_changeSceneTime)
		{
			//���U���g��ʂ�ǂݍ���ł�����
			//������J�����ŎB�e
			//�����\�������Ă������ƂŃV�[���J�ڂ�����

			m_changeScene = true;
		}

		//��������������V�[�������ۂɕς���
		if (m_changeScene)
		{
			return NextScene::ResultScene;
		}

		return NextScene::Continue;
	}

	//�{�̈ʒu�X�V�@�N�[���_�E���^�C�����Ȃ�Ή�ʂɏo���Ȃ�
	m_bookObject.Update();

	m_itemObject.Update();

	//�v���C���[�̓��͍X�V
	//�v���C���[�̈ʒu�X�V
	//�e�̈ʒu�X�V
	m_player_1.Update();
	m_player_2.Update();

	//�ǂ��炩�Е����ꔭ�ł��e���󂯂��ꍇ�Q�[���Z�b�g
	if (m_hitReferee.BulletHitJudge(
		m_player_1.GetCollider(), m_player_2.GetBulletsStructure(), true))
	{
		m_hitSe->Play();//p_1���������ꍇ
		m_gameEnd = true;
		DontDestroy->m_player1Win = true;
	}
	if (m_hitReferee.BulletHitJudge(
		m_player_2.GetCollider(), m_player_1.GetBulletsStructure(), true))
	{
		m_hitSe->Play();//p_2���������ꍇ
		m_gameEnd = true;
		DontDestroy->m_player1Win = false;
	}

	//�A�C�e���ƃv���C���[�����������ꍇ���x����������
	//�{��������x�o������
	if (m_hitReferee.OneHitJudge(m_player_1.GetCollider(), m_itemObject.GetCollider()))
	{
		m_itemObject.Hit();
		m_player_1.ItemHit();
		m_bookObject.RePop();
	}
	if (m_hitReferee.OneHitJudge(m_player_2.GetCollider(), m_itemObject.GetCollider()))
	{
		m_itemObject.Hit();
		m_player_2.ItemHit();
		m_bookObject.RePop();
	}

	//�e�Ɩ{���Ԃ������ꍇ
	if (m_hitReferee.BulletHitJudge(
		m_bookObject.GetCollider(), m_player_1.GetBulletsStructure(), false))
	{
		m_bookObject.Hit();//ture false ��Plyaer1���ۂ��p�ɓn��
		//hit�̒��ōŌ�ɓ��Ă����ʂ֌����������̃t���O���I���ɂ���
		m_itemObject.Pop(m_bookObject.GetPosition(), true);
	}
	if (m_hitReferee.BulletHitJudge(
		m_bookObject.GetCollider(), m_player_2.GetBulletsStructure(), false))
	{
		m_bookObject.Hit();
		m_itemObject.Pop(m_bookObject.GetPosition(), false);
	}


	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);

	ID3D12DescriptorHeap* _heaps[] = { m_resourceDescriptors->Heap() };
	DXTK->CommandList->SetDescriptorHeaps(static_cast<UINT>(std::size(_heaps)), _heaps);

	m_spriteBatch->Begin(DXTK->CommandList);

	auto _spriteBatch = m_spriteBatch.get();

	m_bg.Draw(_spriteBatch);

	m_bookObject.Draw(_spriteBatch);

	m_itemObject.Draw(_spriteBatch);

	m_player_1.PlayerDraw(_spriteBatch);
	m_player_2.PlayerDraw(_spriteBatch);

	m_player_1.BulletDraw(_spriteBatch);
	m_player_2.BulletDraw(_spriteBatch);

	m_player_1.UIDraw(_spriteBatch);
	m_player_2.UIDraw(_spriteBatch);

	m_spriteBatch->End();
	m_mainSceneShader.Render(m_mainToResultWipeSize);



	DXTK->ExecuteCommandList();
}
