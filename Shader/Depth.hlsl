

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// ワールドマトリクス
cbuffer WorldBuffer : register(b0)
{
	matrix world;
}


// ライト
cbuffer LightBuffer : register(b1)
{
	float4 lightPos;
	matrix lightView;
	matrix lightProj;
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture		: register(t0);
SamplerState	g_Sampler		: register(s0);

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
	float4 depth	: TEXCOORD;
};

//=============================================================================
// 頂点シェーダー
//=============================================================================
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;

	float4 pos = mul(float4(In.pos.xyz, 1), world);
	pos = mul(pos, lightView);
	pos = mul(pos, lightProj);

	Out.pos = pos;

	Out.depth = pos;

	return Out;
}


//=============================================================================
// ピクセルシェーダー
//=============================================================================
float4 PS_Main(VS_OUT In) : SV_TARGET
{
	float depth = In.depth.z / In.depth.w;
	return float4(depth, depth * depth, 0.0f, 1.0f);		// rに深度値、gに深度値の２乗を入れとく
}