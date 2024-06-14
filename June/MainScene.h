//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/MainScene/Stage/Stage.h"
#include "Classes/MainScene/Player/Player.h"
#include "Classes/MainScene/Player/PlayerCamera.h"
#include "Classes/MainScene/Enemy/Enemy.h"
#include "Classes/MainScene/ColliderManager.h"
#include "Classes/MainScene/CounterManager.h"
#include "Classes/DescriptorStorage.h"

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

	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;

	DirectXTK::SpriteBatch m_spriteBatch;

	ColliderManager m_colliderManager;

	CounterManager m_counterManager;

	PlayerCamera m_playerCamera;

	Player m_player;

	Enemy m_enemy;

	Stage m_stage;

};