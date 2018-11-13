// グローバル

Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_W : packoffset(c0); // ワールドから射影までの変換行列
	float g_ViewPortWidth : packoffset(c4);// ビューポート（スクリーン）横サイズ
	float g_ViewPortHeight : packoffset(c5);// ビューポート（スクリーン）縦サイズ
};


// 構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};


// バーテックスシェーダー
PS_INPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
	PS_INPUT Out;

	Out.Pos = mul(Pos, g_W);

	Out.Pos.x = (Out.Pos.x / g_ViewPortWidth) * 2 - 1;
	Out.Pos.y = 1 - (Out.Pos.y / g_ViewPortHeight) * 2;

	Out.UV = UV;

	return Out;
}


// ピクセルシェーダー
float4 PS(PS_INPUT Input) : SV_Target
{
	return g_Texture.Sample(g_Sampler, Input.UV);
}