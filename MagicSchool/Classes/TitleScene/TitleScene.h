//
// TemplateScene.h
//

#pragma once

#include "Scenes/Scene.h"


using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class TitleScene final : public Scene {
public:
	TitleScene();
	virtual ~TitleScene() { Terminate(); }

	TitleScene(TitleScene&&) = default;
	TitleScene& operator= (TitleScene&&) = default;

	TitleScene(TitleScene const&) = delete;
	TitleScene& operator= (TitleScene const&) = delete;

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
	// 頂点データ構造体
	struct MyVertex {
		XMFLOAT3 m_pos;
		XMFLOAT2 m_uv;
	};

	// 頂点データ
	static constexpr MyVertex s_screenTriangleVertices[]
	{
		{{ -1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 1.0f,  -1.0f, 0.0f}, {1.0f, 1.0f}},
		{{ -1.0f,  -1.0f, 0.0f}, {0.0f, 1.0f}},
	};
	// インデックスバッファー
	static constexpr  WORD s_screenTriangleIndecies[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	ComPtr<ID3D12Resource>      m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW    m_vertexBufferView;

	ComPtr<ID3D12Resource>      m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW     m_indexBufferView;

	unique_ptr<DescriptorHeap>  m_resourceDescriptors;
	ComPtr<ID3D12Resource>      m_texture;

	//コンスタントバッファ
	ComPtr<ID3D12Resource>		m_constantBuffer;
	ComPtr<ID3D12Resource>		m_constantBuffer2;


	ComPtr<ID3DBlob>            m_vsBlob;
	ComPtr<ID3DBlob>            m_psBlob;


	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	//タイトル画像を映す板ポリを映すカメラ
	DirectXTK::Camera m_camera;



	static constexpr float s_wipeTime = 1.0f;

	const wchar_t* s_sceneChangeSeFilePass =
		L"Sound/SE/title/push_buttn.wav";

	DirectXTK::Sound m_sceneChangeSe;

	DirectXTK::SoundInstance m_sceneChangeSeInstance;

	int m_wipeSpeed;

	float m_wipeCurrentTime;

	bool m_sceneChange;

	bool m_wipeStart;
};