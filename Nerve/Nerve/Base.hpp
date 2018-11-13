// ヘッダーファイルのインクルード
#include <Windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <stdio.h>
#include "Input.hpp"
#include <string>
#include <vector>


// 定数定義
#define APP_NAME L"Nerve"


// マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define SAFE_DEL_RELEASE(x) if(x){delete x; x=NULL;}
#define ALIGN16 _declspec(align(16))
#define DIMENSION 12.0f
#define WDIMENSION 10.0f
#define TEX_DIMENSION 128.0f


// 必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


// 警告非表示
#pragma warning(disable : 4305)

#ifndef _BASE_HPP
#define _BASE_HPP
static int WINDOW_WIDTH = 1920;	//ウィンドウ幅
static int WINDOW_HEIGHT = 1080; //ウィンドウ高さ
#endif // _BASE_HPP
