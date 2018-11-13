#pragma once
#include "Base.hpp"
#include "DebugText.hpp"
#include "Draw.hpp"
#include "Character.hpp"


class Main
{
private:
	DebugText* m_pText;
	Draw* m_pDraw;

	void App();			// アプリケーション処理。アプリのメイン関数。
	void Render();		// シーンを画面にレンダリング

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;

	Character* m_pCharacter;
	

public:
	Main();		// コンストラクタ
	~Main();	// デストラクタ

	HRESULT InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName);		// ウィンドウ作成
	HRESULT InitD3D();																							// Direct3D初期化
	LRESULT MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);										// ウィンドウプロシージャー
	void Run();																									// メッセージループとアプリケーション処理の入り口
	void DestroyD3D();																							// リリース
};
