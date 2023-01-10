//=============================================================================
// �t�@�C��	�F VS_Simple.hlsl
// �쐬��	�F GP11A132 11 �����y��
//-----------------------------------------------------------------------------
//�y�T�v�z
// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��݂̂��s�����_�V�F�[�_�[�ł��B
//=============================================================================


//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// ���[���h�}�g���N�X
cbuffer WorldBuffer : register(b0)
{
	matrix bones[12];
}

// �r���[�}�g���N�X
cbuffer ViewBuffer : register(b1)
{
	matrix view;
}

// �v���W�F�N�V�����}�g���N�X
cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

// �}�e���A���o�b�t�@
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emission;
	float  shininess;
	int    illum;		// �Ɩ����f���i1�ŋ��ʔ��˖���, 2�ŗL���j
	int    noTexSampling;
	int    pad;
};

cbuffer MaterialBuffer : register(b3)
{
	Material mat;
}


//*****************************************************************************
// ���́E�o��
//*****************************************************************************
struct VS_IN
{
	float4 pos		: POSITION;
	float4 nor		: NORMAL;
	float4 col		: COLOR;
	float2 tex		: TEXCOORD;
	uint4  indices	: BLENDINDICES;
	float4 weights	: BLENDWEIGHT;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float4 worldPos	: POSITION1;
	float4 nor		: NORMAL;
	float4 col		: COLOR;
	float2 tex		: TEXCOORD;
};


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture		: register(t0);
SamplerState	g_Sampler		: register(s0);


//=============================================================================
// ���_�V�F�[�_�[
//=============================================================================
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix comb
		= bones[In.indices.x] * In.weights.x
		+ bones[In.indices.y] * In.weights.y
		+ bones[In.indices.z] * In.weights.z
		+ bones[In.indices.w] * In.weights.w;


	float4 pos = float4(In.pos.xyz, 1.0f);
	pos = mul(pos, comb);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	Out.pos = pos;

	Out.worldPos = mul(In.pos, comb);

	Out.nor = normalize(mul(float4(In.nor.xyz, 0.0f), comb));

	Out.col = In.col;

	Out.tex = In.tex;

	return Out;
}

//=============================================================================
// �s�N�Z���V�F�[�_�[
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