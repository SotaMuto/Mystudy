
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
	matrix proj;
}

// マテリアルバッファ
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emission;
	float  shininess;
	int    illum;		// 照明モデル（1で鏡面反射無効, 2で有効）
	int    noTexSampling;
	int    pad;
};

cbuffer MaterialBuffer : register(b3)
{
	Material mat;
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
	float4 pos	: SV_POSITION;
	float4 nor	: NORMAL;
	float4 col	: COLOR;
	float2 tex	: TEXCOORD;
};

//=============================================================================
// 頂点シェーダー
//=============================================================================
VS_OUT VS_Main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;

	float4 pos = float4(In.pos.xyz, 1.0f);
	pos = mul(pos, world);	// ワールド変換
	pos = mul(pos, view);	// ビュー変換
	pos = mul(pos, proj);	// プロジェクション変換
	Out.pos = pos;
	Out.nor = normalize(mul(float4(In.nor.xyz, 0.0f), world));
	Out.col = In.col;
	Out.tex = In.tex;

	return Out;
}

//=============================================================================
// ピクセルシェーダー
//=============================================================================
float4 PS_Main(VS_OUT In) : SV_TARGET
{
	float4 color;

	if (mat.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_Sampler, In.tex);
		color *= In.col;
	}
	else
	{
		color = In.col;
	}

	color *= mat.diffuse;

	return color;
}