#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "MainSceneText.h"

using namespace SimpleMath;

void MainSceneText::CreateText()
{

	// フォントハンドルの生成
	//int fontSize = 32;
	//int fontWeight = 500;
	//LOGFONT lf = {
	//	fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
	//	SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
	//	PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
	//	"ＭＳ Ｐ明朝"
	//};
	//HFONT hFont = CreateFontIndirectA(&lf);

	//// 現在のウィンドウに適用
	//// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	//HDC hdc = GetDC(NULL);
	//HFONT oldFont = (HFONT)SelectObject(hdc, hFont);


	//// ----- ここでGetGlyphOutline関数からビットマップ取得 -----
	//GetGlyphOutline(hdc, "GameClear", GGO_GRAY2_BITMAP, );
	//// ---------------------------------------------------------


	//// デバイスコンテキストとフォントハンドルはもういらないので解放
	//SelectObject(hdc, oldFont);
	//ReleaseDC(NULL, hdc);
}

void MainSceneText::Load(ResourceUploadBatch& esourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& resourceDescriptors)
{
	m_gameClearSprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_gameClearFilePass,
		esourceUpload,
		resourceDescriptors,
		Descriptors::Descriptor::Clear);

	m_gameOverSprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_gameOverFilePass,
		esourceUpload,
		resourceDescriptors,
		Descriptors::Descriptor::GameOver);

	m_operationSprite = DirectXTK::CreateSpriteSRV(
		DXTK->Device,
		s_operationFilePass,
		esourceUpload,
		resourceDescriptors,
		Descriptors::Descriptor::Operation);

}

void MainSceneText::Initialize()
{
	m_gameClearPosition = s_gameClearSpritePosition;

	m_gameOverPosition = s_gameOverSpritePosition;

	m_operationPosition = Vector2(DXTK->Screen.Width / 2 + s_operationSpriteXOffset,
		DXTK->Screen.Height / 2 + s_operationSpriteYOffset);

	m_gameEnd = false;

	m_gameOver = false;
}

void MainSceneText::GameEnd(bool playerIsDeath)
{
	m_gameEnd = true;

	m_gameOver = playerIsDeath;
}

void MainSceneText::Reset()
{
	m_gameClearSprite.resource.Reset();

	m_gameOverSprite.resource.Reset();

	m_operationSprite.resource.Reset();
}

void MainSceneText::Draw(SpriteBatch* batch)
{

	if (m_gameEnd)
	{
		if (m_gameOver)
		{
			batch->Draw(
				m_gameOverSprite.handle, m_gameOverSprite.size, m_gameOverPosition, nullptr,
				Colors::White, s_spriteRotation, g_XMZero, s_gameOverSpriteDrawScale, DirectX::DX12::SpriteEffects_None,
				(float)Descriptors::Descriptor::GameOver / s_layerOffset);
		}
		else
		{
			batch->Draw(
				m_gameClearSprite.handle, m_gameClearSprite.size, m_gameClearPosition, nullptr,
				Colors::White, s_spriteRotation, g_XMZero, s_gameClearSpriteDrawScale, DirectX::DX12::SpriteEffects_None,
				(float)Descriptors::Descriptor::Clear / s_layerOffset);
		}
	}


	batch->Draw(
		m_operationSprite.handle, m_operationSprite.size, m_operationPosition, nullptr,
		Colors::White, s_spriteRotation, g_XMZero, s_operationDrawScale, DirectX::DX12::SpriteEffects_None,
		(float)Descriptors::Descriptor::Operation / s_layerOffset);
}