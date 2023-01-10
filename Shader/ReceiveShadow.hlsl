
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


// カメラ
cbuffer CameraBuffer : register(b4)
{
	float4 camPos;
}

// ライト
cbuffer LightBuffer : register(b5)
{
	float4 lightPos;
	matrix lightView;
	matrix lightProj;
}

// シャドウマップ用パラメータ
struct ShadowMap
{
	int		useVSM;

	float	padding[3];
};

cbuffer ShadowMapBuffer : register(b6)
{
	ShadowMap sm;
}



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
	float4 lightPos	: POSITION1;
	float4 worldPos	: POSITION2;
	float4 nor		: NORMAL0;
	float4 lightRay	: NORMAL1;
	float4 eyePos	: POSITION3;
	float4 col		: COLOR;
	float2 tex		: TEXCOORD;
	float depth		: TEXCOORD1;
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture				: register(t0);
Texture2D		g_ShadowMap				: register(t1);

SamplerState	g_Sampler				: register(s0);
SamplerState	g_ShadowMapSampler		: register(s1);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float VSM_Filter(float2 tex, float fragDepth);
float USM_Filter(float2 tex, float fragDepth);


//=============================================================================
// メイン処理
//=============================================================================
VS_OUT VS_Main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;

	// 座標変換
	float4 pos = float4(In.pos.xyz, 1.0f);
	float4 worldPos = mul(pos, world);
	pos = mul(worldPos, view);
	pos = mul(pos, projection);
	Out.pos = pos;

	// ワールド座標
	Out.worldPos = mul(In.pos, world);

	// ライトの座標変換
	float4 lightSpacePos = mul(worldPos, lightView);
	lightSpacePos = mul(lightSpacePos, lightProj);
	Out.lightPos = lightSpacePos;

	// ライトのベクトル
	Out.lightRay = lightPos;

	// 視線ベクトル
	Out.eyePos = camPos;

	// 法線
	float4 norm = float4(In.nor.xyz, 0.0f);
	norm = mul(norm, world);
	Out.nor = norm;

	Out.col = In.col;
	Out.tex = In.tex;
	Out.depth = length(lightPos.xyz - worldPos.xyz) / 1000.0f;

	return Out;
}


float4 PS_Main(VS_OUT In) : SV_TARGET
{
	float4 color;

	// テクスチャから色をフェッチ
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

	// ハーフランバート
	{
		//float p = saturate(dot(normalize(In.nor), normalize(In.lightRay)));
		//p = p * 0.5f + 0.5f;

		//color *= float4(p, p, p, 1.0f);
	}

	// フォンシェーディング
	{
		float3 normal = normalize(In.nor.xyz);
		float3 lightDir = normalize(In.lightRay.xyz);
		float3 toEye = normalize(In.eyePos.xyz - In.worldPos.xyz);


		// 拡散反射率を求める（ハーフランバート）
		float lambert = saturate(dot(normalize(In.nor), normalize(In.lightRay)));
		lambert = lambert * 0.5f + 0.5f;

		half NdotL = max(0, dot(normal, lightDir.xyz));
		float3 R = normalize(-lightDir + 2.0 * normal * NdotL);
		float3 spec = pow(max(0, dot(R, toEye)), 10.0);

		// 合成
		color.rgb *= lambert + spec;
	}


	//シャドウマップ
	{
		// 影の色
		float shadowColor = 1.0f;

		// テクセルを計算
		float2 texel =
			float2(In.lightPos.x / In.lightPos.w * 0.5f + 0.5f, In.lightPos.y / In.lightPos.w * -0.5f + 0.5f);

		if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1)
		{
			float depth = In.lightPos.z / In.lightPos.w;

			if (sm.useVSM == true)
			{
				float t = VSM_Filter(texel, In.depth);

				float3 shadowCol = (color.rgb * 0.8f);

				color.rgb = lerp(color.rgb, shadowCol, t);
			}
			else
			{
				shadowColor = USM_Filter(texel, depth);

				//影を足す
				color.rgb = color.rgb * shadowColor;
			}
		}

	}

	return color;
}



float VSM_Filter(float2 tex, float fragDepth)
{
	float2 depth = g_ShadowMap.Sample(g_ShadowMapSampler, tex).xy;

	float depth_sq = depth.x * depth.x;
	float variance = depth.y - depth_sq;
	float md = fragDepth - depth.x;
	float p = variance / (variance + (md * md));

	return (max(p, fragDepth >= depth.x));
}


float USM_Filter(float2 tex, float fragDepth)
{
	float depth = g_ShadowMap.Sample(g_ShadowMapSampler, tex).x;

	float lit = 1.0f;
	if (depth < fragDepth - 0.005f)
	{
		lit = 0.8f;
	}

	return lit;
}
