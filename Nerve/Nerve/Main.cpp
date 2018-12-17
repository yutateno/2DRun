#include "Main.hpp"


/// -----------------------------------------------------------------------------------------------------------
// グローバル変数
Main* g_pMain = NULL;


/// -----------------------------------------------------------------------------------------------------------
// 関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/// -----------------------------------------------------------------------------------------------------------
// アプリケーションのエントリー関数 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new Main;
	InputPad::InputPad();
	InputPad::SetPlayerPadNum(0);


	if (g_pMain != NULL)
	{
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			if (SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Run();
			}
		}


		/// アプリ終了
		g_pMain->DestroyD3D();
		delete g_pMain;
	}


	return 0;
}


/// -----------------------------------------------------------------------------------------------------------
// OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}



/// -----------------------------------------------------------------------------------------------------------
Main::Main()
{
	ZeroMemory(this, sizeof(Main));
}


/// -----------------------------------------------------------------------------------------------------------
Main::~Main()
{
}


/// -----------------------------------------------------------------------------------------------------------
HRESULT Main::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize				 = sizeof(wc);
	wc.style				 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc			 = WndProc;
	wc.hInstance			 = hInstance;
	wc.hIcon				 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor				 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground		 = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName		 = WindowName;
	wc.hIconSm				 = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);


	// ウィンドウの作成
	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);
	if (!m_hWnd)
	{
		return E_FAIL;
	}


	// ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}


/// -----------------------------------------------------------------------------------------------------------
LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}


/// -----------------------------------------------------------------------------------------------------------
void Main::Run()
{
	// メッセージループ
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// アプリケーションの処理はここから飛ぶ。
			App();
		}
	}
	// アプリケーションの終了
}


/// -----------------------------------------------------------------------------------------------------------
void Main::App()
{
	InputPad::FirstUpdate();
	Render();
	m_pCharacter->Process();
}


/// -----------------------------------------------------------------------------------------------------------
HRESULT Main::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							 = 1;
	sd.BufferDesc.Width						 = WINDOW_WIDTH;
	sd.BufferDesc.Height					 = WINDOW_HEIGHT;
	sd.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		 = 60;
	sd.BufferDesc.RefreshRate.Denominator	 = 1;
	sd.BufferUsage							 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							 = m_hWnd;
	sd.SampleDesc.Count						 = 1;
	sd.SampleDesc.Quality					 = 0;
	sd.Windowed								 = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}


	// 各種テクスチャーと、それに付帯する各種ビューを作成

	// バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	// デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					 = WINDOW_WIDTH;
	descDepth.Height				 = WINDOW_HEIGHT;
	descDepth.MipLevels				 = 1;
	descDepth.ArraySize				 = 1;
	descDepth.Format				 = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count		 = 1;
	descDepth.SampleDesc.Quality	 = 0;
	descDepth.Usage					 = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags				 = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags		 = 0;
	descDepth.MiscFlags				 = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);
	// そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);


	// レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width		 = static_cast<FLOAT>(WINDOW_WIDTH);
	vp.Height		 = static_cast<FLOAT>(WINDOW_HEIGHT);
	vp.MinDepth		 = 0.0f;
	vp.MaxDepth		 = 1.0f;
	vp.TopLeftX		 = 0;
	vp.TopLeftY		 = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode				 = D3D11_CULL_NONE;
	rdc.FillMode				 = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise	 = TRUE;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);

	
	// 文字列レンダリングの初期化
	m_pText = new DebugText;
	D3DXVECTOR4 vColor(0.9, 0.6, 0, 1);
	if (FAILED(m_pText->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, 50, vColor)))
	{
		return E_FAIL;
	}


	m_pCharacter = new Character;

	m_pMap = new Map;


	// ゲームの素材の初期化
	m_pDraw = new Draw;
	const int fileNum = 12;
	LPCWSTR fileName[] = { L"media\\clph_2d\\scrollaction\\walk\\1.png" , L"media\\clph_2d\\scrollaction\\walk\\2.png"
						 , L"media\\clph_2d\\scrollaction\\walk\\3.png" , L"media\\clph_2d\\scrollaction\\walk\\4.png"
						 , L"media\\clph_2d\\scrollaction\\walk\\5.png" , L"media\\clph_2d\\scrollaction\\walk\\6.png"
						 , L"media\\clph_2d\\scrollaction\\ex\\fall.png" , L"media\\clph_2d\\scrollaction\\ex\\jamp.png"
						 , L"media\\clph_2d\\scrollaction\\ex\\jamp2.png"
						 , L"media\\map\\kabe.png" , L"media\\map\\jumpKabe.png" , L"media\\map\\kabeDead.png" };
	if (FAILED(m_pDraw->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, fileName, fileNum, 64, 64)))
	{
		return E_FAIL;
	}


	// フルスクリーンにする（DirectX11なのでALT+Enterで切り替え可
	m_pSwapChain->SetFullscreenState(true, 0);


	return S_OK;
}


/// -----------------------------------------------------------------------------------------------------------
void Main::Render()
{
	/// 画面クリア
	float ClearColor[4] = { 0,1,1,1 };	// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);						// カラーバッファクリア
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);	// デプスステンシルバッファクリア


	// FPS表示
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	static char str[256];
	if (timeGetTime() - time > 1000)
	{
		sprintf(str, "fps=%d", frame);
		time = timeGetTime();
		frame = 0;
	}
	m_pText->Render(str, 0, 10);


	// キャラクター描画
	m_pDraw->Render(m_pCharacter->GetID(), m_pCharacter->GetX(), m_pCharacter->GetY()
		, m_pCharacter->GetXSize(), m_pCharacter->GetYSize(), m_pCharacter->GetRightDire(), false);



	// マップ描画
	for (int i = 0, n = m_pMap->GetMapID().size(); i != n; ++i)
	{
		for (int j = 0, m = m_pMap->GetMapID()[i].size(); j != m; ++j)
		{
			float tempX = j * m_pMap->GetSpriteSize() - m_pCharacter->GetMapDrawAddX();
			float tempY = i * m_pMap->GetSpriteSize();
			if (tempX > BASE_WIDTH || tempX < -64 || tempY > BASE_HEIGHT || tempY < -64)
			{
				continue;
			}

			switch (m_pMap->GetMapID()[i][j])
			{
			case 1:
				m_pDraw->Render(static_cast<int>(MAPID::wall), tempX, tempY
					, m_pMap->GetSpriteSize(), m_pMap->GetSpriteSize(), false, false);
				break;

			case 0:
				m_pDraw->Render(static_cast<int>(MAPID::wallJump), tempX, tempY
					, m_pMap->GetSpriteSize(), m_pMap->GetSpriteSize(), false, false);
				break;

			case 2:
				m_pDraw->Render(static_cast<int>(MAPID::wallDead), tempX, tempY
					, m_pMap->GetSpriteSize(), m_pMap->GetSpriteSize(), false, false);
				break;

			default:
				break;
			}
		}
	}

	m_pSwapChain->Present(1, 0);	// 画面更新（バックバッファをフロントバッファに）
}


/// -----------------------------------------------------------------------------------------------------------
void Main::DestroyD3D()
{
	SAFE_DEL_RELEASE(m_pDraw);
	SAFE_DEL_RELEASE(m_pMap);
	SAFE_DEL_RELEASE(m_pCharacter);
	SAFE_DEL_RELEASE(m_pText);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
