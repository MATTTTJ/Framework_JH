
#include "Shader_Client_Defines.h"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
/* 1. 모델의 전체의 뼈를 받아온다. */
/* 2. 모델 중, 현재 그릴려고 하는 메시에 뼈를 받아온다. */
matrix			g_BoneMatrices[256];
float3			g_vCameraPos;
texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	/* 현재 정점에게 곱해져야할 뼈들(최대 4개)의 행렬을 만든다. */
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION; // SV_POSITION
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vTangent : TANGENT;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;


	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 정점의 Weight.x + y + z + w = 1.f */

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector			vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	vector			vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);

	
	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vTangent = (vector)0.f;

	return Out;
}



struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vTangent : TANGENT;
};



struct PS_OUT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vColor : SV_TARGET0;
};

//PS_OUT
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4 TexNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);

	float3 vCameraPos = normalize(TexNormal.xyz - g_vCameraPos.xyz);

	// float RimLightColor = smoothstep(0.0001f, 1.f, 1 - saturate(dot(In.vNormal.xyz, vCameraPos.xyz)));
	float RimLightColor = 1 - saturate(dot(In.vNormal.xyz, -vCameraPos.xyz));

	RimLightColor = pow(RimLightColor, 100.0f);

	float Lx = 0;
	float Ly = 0;

	for( int y = -1; y <= 1; ++y)
	{
		for(int x = -1; x<= 1; ++x)
		{
			float2 offset = float2(x, y) * float2(1/1280.f, 1/720.f);
			float3 tex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV + offset).rgb;
			float luminance = dot(tex, float3(0.3, 0.59, 0.11));

			Lx += luminance * Kx[y + 1][x + 1];
			Ly += luminance * Ky[y + 1][x + 1];
		}
	}
	float L = sqrt((Lx*Lx) + (Ly*Ly));

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) + (RimLightColor * 0.15f);


	// if(L < 0.05)
	// {
	// 	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) - RimLightColor;
	// }
	// else
	// {
	// 	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) * 0.6f ;
	// }
	// Out.vColor = float4(L.xxx, 1);
	//
	// if(Out.vColor.rgb / 3.f < 0.99f)
	// {
	// 	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	// }
	// else
	// 	Out.vColor = float4(L.xxx, 1);
	if (0.1f > Out.vColor.a)
		discard;

	
	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);
		

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
