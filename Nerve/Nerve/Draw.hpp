#pragma once
#include "Base.hpp"


// 頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR2 UV; //テクスチャー座標
};


// シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX mW;
	ALIGN16 float ViewPortWidth;
	ALIGN16 float ViewPortHeight;
};


class Draw
{
private:
	HRESULT InitModel(float xSize, float ySize);
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	void RenderSprite(D3DXMATRIX& World, ID3D11ShaderResourceView* pTexture);

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;

	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;
	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	std::vector<ID3D11ShaderResourceView*> m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
	ID3D11BlendState* m_pBlendState;


public:
	Draw();
	~Draw();
	HRESULT Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height, LPCWSTR* p_fileName, const int num, float xSize, float ySize);
	void Render(int num, float x, float y, float xSize, float ySize, bool flipHorizontal, bool flipVertica);
};
