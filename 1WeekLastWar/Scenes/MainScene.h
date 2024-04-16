//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/Player/Player/Player.h"
#include "Classes/Stage/Stage.h"
#include "Classes/Enemy/EnemyManeger.h"
#include "Classes/Item/ItemManeger.h"
#include "Classes/MainScene/HitJudgment.h"

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

	DirectXTK::Camera m_camera;

	SimpleMath::Vector3 m_viewPositon;

	Stage m_stage;

	Player m_player;

	EnemyManeger m_enemyManeger;

	ItemManeger m_itemManeger;

	HitJudgment m_hitHudgment;

};