#include "DebugText.hpp"



DebugText::DebugText()
{
	ZeroMemory(this, sizeof(DebugText));
	m_fScale = 1.0f;
}


DebugText::~DebugText()
{
}


HRESULT DebugText::Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height, float size, D3DXVECTOR4 vColor)
{
	m_fAlpha = vColor.w;
	m_vColor = vColor;
	m_fScale = size / 25;

	for (int i = 0; i < 95; i++)
	{
		m_fKerning[i] = 10;
	}
	//デバイスとコンテキストをコピー
	m_pDeviceContext = pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);
	//window サイズ
	m_dwWindowWidth = width;
	m_dwWindowHeight = height;
	//フォントごとにクアッド作成
	float left = 0, top = 0, right = 0, bottom = 0;
	int cnt = 0;
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			left = k * WDIMENSION;
			top = i * DIMENSION;
			right = left + m_fKerning[cnt];
			bottom = top + DIMENSION;
			left /= TEX_DIMENSION;
			top /= TEX_DIMENSION;
			right /= TEX_DIMENSION;
			bottom /= TEX_DIMENSION;
			TextVertex vertices[] =
			{
				D3DXVECTOR3(0,DIMENSION,0),D3DXVECTOR2(left,bottom),//頂点1,
				D3DXVECTOR3(0,0,0), D3DXVECTOR2(left,top),//頂点2
				D3DXVECTOR3(WDIMENSION,DIMENSION,0),D3DXVECTOR2(right,bottom), //頂点3
				D3DXVECTOR3(WDIMENSION,0,0),D3DXVECTOR2(right,top), //頂点4
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(TextVertex) * 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer[cnt])))
			{
				return E_FAIL;
			}
			cnt++;
		}
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
	//フォントのテクスチャーを作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"media\\ascii.png", NULL, NULL, &m_pAsciiTexture, NULL)))
	{
		return E_FAIL;
	}
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"SpriteText.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return FALSE;
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"SpriteText.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(TEXT_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	//抜け色設定
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendState)))
	{
		return E_FAIL;
	}

	return S_OK;
}


void DebugText::Render(char* text, int x, int y)
{
	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&m_mView, &vEyePt, &vLookatPt, &vUpVec);
	// プロジェクショントランスフォーム（射影変換）
	D3DMATRIX mOtho = {
		2.0f / (float)(m_dwWindowWidth), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(m_dwWindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_mProj = mOtho;
	//トポロジー
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pAsciiTexture);

	//render
	for (int i = 0; i < strlen(text); i++)
	{
		char font = text[i];
		int index = font - 32;
		RenderFont(index, x, y);
		x += m_fKerning[index];
	}
}


void DebugText::RenderFont(int FontIndex, int x, int y)
{
	//ワールド変換
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mWorld, x, y, -100);
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	TEXT_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = mWorld * m_mView*m_mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//カラーを渡す
		cb.vColor = m_vColor;
		//透明度を渡す
		cb.fAlpha.x = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer[FontIndex], &stride, &offset);

	//抜け色
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
	m_pDeviceContext->OMSetBlendState(0, NULL, ColorKey);
}