//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include "Classes/Descriptors .h"
#include "Classes/BackGrond.h"
#include "Classes/Player/Player.h"
#include "Classes/HitReferee.h"
#include "Classes/MainScene/MainSceneShader.h"
#include "Classes/MainScene/Book/BookObject.h"
#include "Classes/MainScene/Book/ItemObject.h"
#include "DontDestroyOnLoad.h"


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
	Descriptors m_descriptors;

	HitReferee m_hitReferee;

	DirectXTK::DescriptorHeap m_resourceDescriptors;

	DirectXTK::SpriteBatch m_spriteBatch;

	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;

	BackGrond m_bg;

	MainSceneShader m_mainSceneShader;

	Player m_player_1;
	Player m_player_2;

	BookObject m_bookObject;

	ItemObject m_itemObject;

	const wchar_t* s_hitSeFilePass =
		L"Sound/SE/battle/player_hit.wav";

	DirectXTK::Sound m_hitSe;

	float m_mainToResultWipeSize;

	static constexpr float s_changeSceneTime = 3.0f;
	float m_changeSceneCurrentTime;

	bool m_changeScene;

	bool m_gameEnd;

};