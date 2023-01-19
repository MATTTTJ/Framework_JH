#include "Shader_Client_Defines.h"
bool			g_bNormalTexOn = false;
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_SocketMatrix;

texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float4		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));

	return Out;
}


VS_OUT VS_MAIN_SOCKET(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;


	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	vector		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_SocketMatrix);

	vector		vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
	vNormal = mul(vNormal, g_SocketMatrix);

	Out.vPosition = mul(vPosition, matVP); 
	Out.vNormal = normalize(vNormal);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
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
};
// vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
// if (0.1f > vDiffuse.a)
// 	discard;
// Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
//
// float Lx = 0;
// float Ly = 0;
//
// for (int y = -1; y <= 1; ++y)
// {
// 	for (int x = -1; x <= 1; ++x)
// 	{
// 		float2 offset = float2(x, y) * float2(1 / 1280.f, 1 / 720.f);
// 		float3 tex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV + offset).rgb;
// 		float luminance = dot(tex, float3(0.3, 0.59, 0.11));
//
// 		Lx += luminance * Kx[y + 1][x + 1];
// 		Ly += luminance * Ky[y + 1][x + 1];
// 	}
// }
// float L = sqrt((Lx*Lx) + (Ly*Ly));
// if (L < 0.1)
// {
// 	Out.vDiffuse = vDiffuse;
// 	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
// }
// else
// {
// 	Out.vDiffuse = vDiffuse * 0.3f;
// 	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
// }
//
//
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	// if (0.1f > vDiffuse.a)
	// 	discard;

	vDiffuse.a = 1.f;

	if (g_bNormalTexOn)
	{
		vector		SwapNormal;
		vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);

		SwapNormal.x = vNormalDesc.y;
		SwapNormal.y = In.vNormal.y;
		SwapNormal.z = vNormalDesc.x;
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

	pass Socket
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SOCKET();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
