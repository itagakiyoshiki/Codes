//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/Descriptors .h"
#include "Classes/Stage/Stage.h"
#include "Classes/Player/Player.h"
#include "Classes/Enemy/Enemy.h"
#include "Classes/MainScene/MainSceneText.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainScene final : public Scene {
public:
	MainScene();
	virtual ~MainScene() { Terminate(); }

	MainScene(MainScene&&) = default;
	MainScene& operator= (MainScene&&) = default;

	MainScene(MainScene const&) = delete;
	MainScene& operator= (MainScene const&) = delete;

	// These are the functions you will implement.
	void Start() override;
	void Initialize() override;
	void LoadAssets() override;

	void Terminate() override;

	void OnDeviceLost() override;
	void OnRestartSound() override;

	NextScene Update(const float deltaTime) override;
	void Render() override;

private:

	const wchar_t* s_mainBGMFilePass =
		L"BGM/AnyConv.com__GameMainBGM.wav";

	const wchar_t* s_gameClearBGMFilePass =
		L"BGM/AnyConv.com__GameClear.wav";

	const wchar_t* s_gameOverBGMFilePass =
		L"BGM/AnyConv.com__GameOver.wav";

	static constexpr SimpleMath::Vector3 s_defaultPosition =
		SimpleMath::Vector3(0.0f, 0.0f, -30);

	static constexpr SimpleMath::Vector3 s_defaultRotation =
		SimpleMath::Vector3(0.0f, -1.0f, 0);

	static constexpr float s_viewAngle = Mathf::PI / 4.0f;
	static constexpr float s_nearz = 0.1f;
	static constexpr float s_farz = 10000.0f;

	static constexpr float s_hitDistance = 0.2f;

	DirectXTK::Camera m_camera;

	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

	DirectXTK::SpriteBatch m_spriteBatch;


	DirectXTK::Sound          m_mainBgm;
	DirectXTK::Sound          m_gameClearBgm;
	DirectXTK::Sound          m_gameOverBgm;
	DirectXTK::SoundInstance  m_bgmInstance;

	Descriptors m_descriptors;

	MainSceneText m_mainSceneText;

	Stage m_stage;

	Player m_player;

	Enemy m_enemy;

	bool m_gameEnd;
};