//
// TemplateScene.h
//

#pragma once

#include "Scenes/Scene.h"
#include "Classes/Descriptors .h"
#include "Classes/BackGrond.h"
#include "Classes/ResultScene/RetryButton.h"
#include "Classes/ResultScene/GameEndButton.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class ResultScene final : public Scene {
public:
	ResultScene();
	virtual ~ResultScene() { Terminate(); }

	ResultScene(ResultScene&&) = default;
	ResultScene& operator= (ResultScene&&) = default;

	ResultScene(ResultScene const&) = delete;
	ResultScene& operator= (ResultScene const&) = delete;

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

	enum LoadScene
	{
		Stay, Title, Result
	};

	LoadScene m_loadScene;

	DirectXTK::DescriptorHeap m_resourceDescriptors;

	DirectXTK::SpriteBatch m_spriteBatch;

	Descriptors m_descriptors;

	BackGrond m_bg;

	RetryButton m_retryButton;

	GameEndButton m_gameEndButton;



};