
#include "Shader_Client_Defines.h"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
/* 1. 모델의 전체의 뼈를 받아온다. */
/* 2. 모델 중, 현재 그릴려고 하는 메시에 뼈를 받아온다. */
matrix			g_BoneMatrices[512];
float3			g_vCameraPos;
texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;
texture2D		g_MonsterNormalTexture;

texture2D		g_ModelGlowTexture;
texture2D		g_ModelTestTexture;
texture2D		g_ModelSpecularTexture;
float4			g_vColor = float4(0.f, 0.f, 0.f,0.f);
bool			g_bNormalTexOn;
bool			g_bSpecularTexOn;
bool			g_bHit = false;
float4			g_vLimColor; 
float			g_Outline_Offset = 0.03f;
float			g_fFar = 300.f;
texture2D       g_OutLineTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION; // SV_POSITION
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float4		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

float4 CreateOutline(float4 vertPos, float Outline)
{
	float4x4 scaleMat;
	scaleMat[0][0] = 1.0f + Outline;
	scaleMat[0][1] = 0.0f;
	scaleMat[0][2] = 0.0f;
	scaleMat[0][3] = 0.0f;
	scaleMat[1][0] = 0.0f;
	scaleMat[1][1] = 1.0f + Outline;
	scaleMat[1][2] = 0.0f;
	scaleMat[1][3] = 0.0f;
	scaleMat[2][0] = 0.0f;
	scaleMat[2][1] = 0.0f;
	scaleMat[2][2] = 1.0f + Outline;
	scaleMat[2][3] = 0.0f;
	scaleMat[3][0] = 0.0f;
	scaleMat[3][1] = 0.05f;
	scaleMat[3][2] = 0.0f;
	scaleMat[3][3] = 1.0f;

	return mul(scaleMat, vertPos);
}

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
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));

	return Out;
}

VS_OUT VS_MAIN_OUTLINE(VS_IN In)
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

	In.vPosition.y = In.vPosition.y * 0.5f;
	vector			vTest = mul(float4(In.vPosition, 1.f), BoneMatrix);
	
	Out.vPosition = mul(vPosition + vNormal * g_Outline_Offset, matWVP);
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
	float4		vProjPos : TEXCOORD1;
	float4		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

struct PS_OUT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
	float4		vFlag : SV_TARGET3;
};

struct PS_OUT_UNNORM
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

//PS_OUT
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.1f > vDiffuse.a)
		discard;
	vDiffuse.a = 1.f;


	if (g_bNormalTexOn)
	{
		vector		SwapNormal;
		vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
		SwapNormal.x = vNormalDesc.z;
		SwapNormal.y = vNormalDesc.y;
		SwapNormal.z = vNormalDesc.x;
		SwapNormal.w = 0;

		float3		vNormal = SwapNormal.xyz * 2.f - 1.f;
		float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal, In.vNormal.xyz);
		vNormal = normalize(mul(vNormal, WorldMatrix));
		Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
		// Out.vNormal = SwapNormal;
	}
	else
	{
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	}

	// vector OutlineTex = g_OutLineTexture.Sample(LinearSampler, In.vTexUV);
	//
	// if(OutlineTex.a == 1.f)
	// {
	// 	Out.vDiffuse = vDiffuse + OutlineTex;
	// }
	// else
		Out.vDiffuse = vDiffuse;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	return Out;
}

PS_OUT_UNNORM PS_MAIN_Boss(PS_IN In)
{
	PS_OUT_UNNORM			Out = (PS_OUT_UNNORM)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.1f > vDiffuse.a)
		discard;

	vDiffuse.a = 1.f;
	
	vector		vNormalDesc = g_MonsterNormalTexture.Sample(LinearSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal, In.vNormal.xyz);
	vNormal = normalize(mul(vNormal, WorldMatrix));
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDiffuse = vDiffuse;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	return Out;
}

PS_OUT PS_MAIN_Monster_Normal(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.1f > vDiffuse.a)
		discard;

	vDiffuse.a = 1.f;


	if (g_bNormalTexOn)
	{
		vector		SwapNormal;
		vector		vNormalDesc = g_MonsterNormalTexture.Sample(LinearSampler, In.vTexUV);
		SwapNormal.x = vNormalDesc.x;
		SwapNormal.y = vNormalDesc.y;
		SwapNormal.z = vNormalDesc.z;
		SwapNormal.w = 0;

		float3		vNormal = SwapNormal.xyz * 2.f - 1.f;
		float3x3	WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal, In.vNormal.xyz);
		vNormal = normalize(mul(vNormal, WorldMatrix));
		Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	}
	else
	{
		Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	}

	Out.vDiffuse = vDiffuse;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);

	return Out;
}

PS_OUT_UNNORM PS_MAIN_Monster_UnNormal(PS_IN In)
{
	PS_OUT_UNNORM			Out = (PS_OUT_UNNORM)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	// if (0.1f > vDiffuse.a)
	// 	discard;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	Out.vDiffuse = vDiffuse;

	return Out;
}


PS_OUT PS_MAIN_OUTLINE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (0.1f > vDiffuse.a)
		discard;
		
	if (g_OutLineColor.r > 0.5f)
	{
		Out.vDiffuse = g_OutLineColor;
		Out.vNormal = vector(0.f, 1.f, 0.f, 0.f);
		Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	}
	else
	{
		Out.vDiffuse = vector(0.f, 0.f, 0.f, 1.f);
		Out.vNormal = vector(0.f, 0.f, 0.f, 0.f);
		Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.f, 0.f);
	}
	
	if (Out.vDiffuse.a < 0.1f)
		discard;

	return Out;
}

struct PS_OUT_FLAG
{
	vector		vFlag : SV_TARGET0;
};

PS_OUT_FLAG PS_OUTLINE(PS_IN In)
{
	PS_OUT_FLAG		Out = (PS_OUT_FLAG)0;
	//외곽선 유무
	Out.vFlag = vector(g_vColor.rgb, 1.f);

	return Out;
}

struct VS_IN_SHADOW
{
	float3		vPosition : POSITION;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT_SHADOW
{
	float4		vPosition : SV_POSITION; // SV_POSITION
	float4		vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW		Out = (VS_OUT_SHADOW)0;


	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 정점의 Weight.x + y + z + w = 1.f */

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector			vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_SHADOW
{
	float4		vPosition : SV_POSITION;
	float4		vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW			Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.z;

	Out.vLightDepth.a = 1.f;

	return Out;
}





technique11 DefaultTechnique
{
	pass PlayerArm0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);
		

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	
	pass OUTLINE1
	{
		SetRasterizerState(RS_CW);
		SetDepthStencilState(DS_Test, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN_OUTLINE();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OUTLINE();
	}

	pass MonsterNormal2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Monster_Normal();
	}

	pass MonsterUnNormal3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Monster_UnNormal();
	}

	pass MonsterBoss4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Boss();
	}

	pass ShadowDepth5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}

	pass Outline6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE();
	}
}
