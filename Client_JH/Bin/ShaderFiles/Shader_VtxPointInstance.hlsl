
#include "Shader_Client_Defines.h"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_SocketMatrix;
float			g_fTrailCount;
float2			g_vPSize;
vector			g_vCamPosition;
vector			g_vInitPos;
vector			g_vLook;
vector			g_vRightSrc;
vector			g_vUp;

float			g_fProgress =1.f;
float			g_fPreProgress = 1.f;
float			g_fTest; 
texture2D		g_Texture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	row_major float4x4	Matrix : WORLD;	
};

struct VS_OUT
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;
};

/* 추가적으로 정점을 생성할 수 있는 상황을 준비해준다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), In.Matrix);
	// vPosition = mul(vPosition, g_SocketMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix).xyz;

	Out.vPSize = In.vPSize;

	return Out;
}

struct GS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;
};


struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN( point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	GS_OUT		Out[4];

	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * g_vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * g_vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;



	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(0.f, 0.f);

	vPosition = In[0].vPosition - vRight + vUp;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f, 0.f);

	vPosition = In[0].vPosition - vRight - vUp;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f, 1.f);

	vPosition = In[0].vPosition + vRight - vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(0.f, 1.f);

	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
};

[maxvertexcount(6)]
void GS_MAIN_UI_Red(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	GS_OUT		Out[4];

	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook))* g_vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * g_vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	// In[0].vPosition.x = In[0].vPosition.x - (vRight * g_fTest) ;
	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2((1-g_fProgress) * 0.5f, 0.f);

	vPosition = ((In[0].vPosition - vRight  )+ vUp);
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(0.5f + (1-g_fProgress)*0.5f , 0.f);

	vPosition = ((In[0].vPosition - vRight )- vUp);
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(0.5f + (1 - g_fProgress)*0.5f, 1.f);

	vPosition = In[0].vPosition + (vRight)- vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2((1 - g_fProgress) * 0.5f, 1.f);

	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
};


[maxvertexcount(6)]
void GS_MAIN_UI_White(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	GS_OUT		Out[4];

	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook))* g_vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * g_vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	// float Lerp = smoothstep((g_fPreProgress - 0.2f), g_fPreProgress, (g_fProgress)) - smoothstep((g_fPreProgress), g_fPreProgress + 0.2f, (g_fProgress));
	// In[0].vPosition.x = In[0].vPosition.x - (vRight * g_fTest) ;
	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2((1 - g_fProgress) * 0.5f, 0.f);

	vPosition = ((In[0].vPosition - vRight) + vUp);
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(0.5f + (1 - g_fProgress)*0.5f, 0.f);

	vPosition = ((In[0].vPosition - vRight) - vUp);
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(0.5f + (1 - g_fProgress)*0.5f, 1.f);

	vPosition = In[0].vPosition + (vRight)-vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2((1 - g_fProgress) * 0.5f, 1.f);

	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
};


[maxvertexcount(6)]
void GS_MAIN_BULLET(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	GS_OUT		Out[4];


	float3		vLook = g_vLook.xyz;

	float3		vRight = normalize(cross(g_vUp.xyz, vLook)* In[0].vPSize.x * 0.5f);

	float3		vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(0.f, 0.f);

	vPosition = (In[0].vPosition - vRight + vUp) ;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f, 0.f);

	vPosition = (In[0].vPosition - vRight - vUp) ;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f, 1.f);

	vPosition = In[0].vPosition + vRight - vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(0.f, 1.f);
	
	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();

};

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	// Out.vColor.rgb = float3(1.f, 0.f, 0.f);
	//
	if(Out.vColor.a < 0.1f)
		discard;
	//
	return Out;
}

PS_OUT PS_MAIN_MonsterUI(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	// Out.vColor.a = g_fProgress;

	// if (Out.vColor.a < 1 - g_fProgress)
	// 	discard;
	// 텍스쳐 UV이가 몇이냐에 따라 어떤 색을섞을지 ?

	// Out.vColor.rgb = float3(1.f, 0.f, 0.f);

	// if (Out.vColor.a < 0.1f)
	// 	discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Rect0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Bullet1
	{
		SetRasterizerState(RS_None);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN_BULLET();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Monster2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Test_UI_RED, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MonsterUI();
	}

	pass Monster_UI_Red3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN_UI_Red();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MonsterUI();
	}

	pass Monster_UI_White4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Test_UI_RED, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN_UI_White();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MonsterUI();
	}
}
