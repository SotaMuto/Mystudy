

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// ���[���h�}�g���N�X
cbuffer WorldBuffer : register(b0)
{
	matrix world;
}


// ���C�g
cbuffer LightBuffer : register(b1)
{
	float4 lightPos;
	matrix lightView;
	matrix lightProj;
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
	float4 pos		: SV_POSITION;
	float4 depth	: TEXCOORD;
};

//=============================================================================
// ���_�V�F�[�_�[
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
// �s�N�Z���V�F�[�_�[
//=============================================================================
float4 PS_Main(VS_OUT In) : SV_TARGET
{
	float depth = In.depth.z / In.depth.w;
	return float4(depth, depth * depth, 0.0f, 1.0f);		// r�ɐ[�x�l�Ag�ɐ[�x�l�̂Q������Ƃ�
}