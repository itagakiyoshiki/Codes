#pragma once


#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "Scenes/Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainSceneShader
{
public:

	void Initialize();

	void LoadAssets();

	void Terminate();

	void Render(float);

private:
	struct MyVertex {
		XMFLOAT3 m_pos;
		XMFLOAT2 m_uv;
	};

	static constexpr MyVertex s_screenTriangleVertices[]
	{
		{{ -1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 1.0f,  -1.0f, 0.0f}, {1.0f, 1.0f}},
		{{ -1.0f,  -1.0f, 0.0f}, {0.0f, 1.0f}},
	};

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

	const wchar_t* s_resultSpriteFilePass =
		L"Sprite/GameOver/rizaruto.png";

	static constexpr float s_wipeTime = 1.0f;
};

