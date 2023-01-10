//=============================================================================
// ファイル	： VS_Simple.hlsl
// 作成者	： GP11A132 11 小峰遥一
//-----------------------------------------------------------------------------
//【概要】
// ワールド・ビュー・プロジェクション変換のみを行う頂点シェーダーです。
//=============================================================================

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// ワールドマトリクス
cbuffer WorldBuffer : register(b0)
{
	matrix world;
}

// ビューマトリクス
cbuffer ViewBuffer : register(b1)
{
	matrix view;
}

// プロジェクションマトリクス
cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

// ブラー用バッファ
cbuffer BlurBuffer : register(b3)
{
	float2 texelSize;
	float2 offset;

	float weight0;
	float weight1;
	float weight2;
	float weight3;
	float weight4;
	float weight5;
	float weight6;
	float weight7;
};

//*****************************************************************************
// 入力・出力
//*****************************************************************************
struct VS_IN
{
	float4 pos	: POSITION;
	float4 nor	: NORMAL;
	float4 col	: COLOR;
	float2 tex	: TEXCOORD;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float4 nor		: NORMAL;
	float4 col		: COLOR;

	float2 texel[8] : TEXCOORD0;
};


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture		: register(t0);
SamplerState	g_Sampler		: register(s0);



/**
* @brief ピクセルシェーダー
*/
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;

	float4 pos = float4(In.pos.xyz, 1.0f);
	pos = mul(pos, world);			// ワールド変換
	pos = mul(pos, view);			// ビュー変換
	pos = mul(pos, projection);		// プロジェクション変換

	Out.pos = pos;
	Out.nor = normalize(mul(float4(In.nor.xyz, 0.0f), world));
	Out.col = In.col;

	Out.texel[0] = In.tex - texelSize * 1.0f;         // 1.0f / TextureSize
	Out.texel[1] = In.tex - texelSize * 3.0f;         // 3.0f / TextureSize
	Out.texel[2] = In.tex - texelSize * 5.0f;         // 5.0f / TextureSize
	Out.texel[3] = In.tex - texelSize * 7.0f;         // 7.0f / TextureSize
	Out.texel[4] = In.tex - texelSize * 9.0f;         // 9.0f / TextureSize
	Out.texel[5] = In.tex - texelSize * 11.0f;        // 11.0f / TextureSize
	Out.texel[6] = In.tex - texelSize * 13.0f;        // 13.0f / TextureSize
	Out.texel[7] = In.tex - texelSize * 15.0f;        // 15.0f / TextureSize
	
	return Out;
}


/**
* @brief ピクセルシェーダー
*/
float4 PS_Main(VS_OUT In) : SV_Target0
{
   float4 color = 0.0f;

   color += (g_Texture.Sample(g_Sampler, In.texel[0]) + g_Texture.Sample(g_Sampler, In.texel[7] + offset)) * weight0;   // 1.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[1]) + g_Texture.Sample(g_Sampler, In.texel[6] + offset)) * weight1;   // 3.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[2]) + g_Texture.Sample(g_Sampler, In.texel[5] + offset)) * weight2;   // 5.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[3]) + g_Texture.Sample(g_Sampler, In.texel[4] + offset)) * weight3;   // 7.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[4]) + g_Texture.Sample(g_Sampler, In.texel[3] + offset)) * weight4;   // 9.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[5]) + g_Texture.Sample(g_Sampler, In.texel[2] + offset)) * weight5;   // 11.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[6]) + g_Texture.Sample(g_Sampler, In.texel[1] + offset)) * weight6;   // 13.0f / TextureSize
   color += (g_Texture.Sample(g_Sampler, In.texel[7]) + g_Texture.Sample(g_Sampler, In.texel[0] + offset)) * weight7;   // 15.0f / TextureSize

   return color;
}
