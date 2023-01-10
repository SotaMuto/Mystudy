
//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// ���[���h�}�g���N�X
cbuffer WorldBuffer : register(b0)
{
	matrix world;
}

// �r���[�}�g���N�X
cbuffer ViewBuffer : register(b1)
{
	matrix view;
}

// �v���W�F�N�V�����}�g���N�X
cbuffer ProjectionBuffer : register(b2)
{
	matrix proj;
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
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture		: register(t0);
SamplerState	g_Sampler		: register(s0);


//*****************************************************************************
// ���́E�o��
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
// ���_�V�F�[�_�[
//=============================================================================
VS_OUT VS_Main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;

	float4 pos = float4(In.pos.xyz, 1.0f);
	pos = mul(pos, world);	// ���[���h�ϊ�
	pos = mul(pos, view);	// �r���[�ϊ�
	pos = mul(pos, proj);	// �v���W�F�N�V�����ϊ�
	Out.pos = pos;
	Out.nor = normalize(mul(float4(In.nor.xyz, 0.0f), world));
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