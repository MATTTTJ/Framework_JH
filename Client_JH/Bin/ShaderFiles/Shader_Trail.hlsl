#include "Shader_Client_Defines.h"
#include "Shader_Function.h"

/*
g_fLife : 전체 유지 시간 길이
fLife   : 해당 인스턴스의 진행된 시간
*/
matrix      g_matWorld, g_matView, g_matProj;
matrix      g_matInfo[300];
float		g_fWidth = 1.f;
float       g_fLife;
float4      g_vColor;
bool        g_bDistanceAlpha;

texture2D   g_DiffuseTexture;

struct VS_IN
{
    float3      vPosition   : POSITION;
    float2      vPSize     : PSIZE;
    row_major   float4x4    Matrix  : WORLD;
    uint        InstanceID  : SV_InstanceID;
};

struct VS_OUT
{
    float4      vPosition   : POSITION;
    float2      vPSize      : PSIZE;
    float       fLife       : TEXCOORD0;
    row_major   float4x4    Matrix  : WORLD;
    uint        InstanceID  : SV_InstanceID;

};

VS_OUT  VS_MAIN(VS_IN In)
{
    VS_OUT  Out = (VS_OUT)0;

    float4x4    Matrix = In.Matrix;

    Matrix[0][3] = 0.f;
    Matrix[1][3] = 0.f;
    Matrix[2][3] = 0.f;
    Matrix[3][3] = 1.f;

    Out.vPosition = float4(In.vPosition, 1.f);
    Out.vPSize = In.vPSize;
    Out.fLife = In.Matrix[3][3];
    Out.Matrix = Matrix;
    Out.InstanceID = In.InstanceID;

    return Out;
}

struct GS_IN
{
    float4      vPosition   : POSITION;
    float2      vPSize      : PSIZE;
    float       fLife       : TEXCOORD0;
    row_major   float4x4    Matrix  : WORLD;
    uint        InstanceID  : SV_InstanceID;
};

struct GS_OUT
{
    float4      vPosition   : SV_Position;
    float2      vTexUV      : TEXCOORD0;
    float       fLife       : TEXCOORD1;
};

[maxvertexcount(6)]
void  GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
    GS_OUT  Out[4];

    matrix  matWV = mul(g_matWorld, g_matView);
    matrix  matWVP = mul(matWV, g_matProj);
    matrix  matVP = mul(g_matView, g_matProj);

    float   fCurWidth = g_fWidth * In[0].fLife / g_fLife * 0.5f;
    float3  vUp = normalize(matrix_up(In[0].Matrix));

	vector		vPosition = vector(matrix_postion(In[0].Matrix), 1.f);
    vector      vResultPos;

    if (In[0].InstanceID == 0)
    {
		vResultPos = vPosition + float4(vUp, 0.f) * fCurWidth;
        Out[0].vPosition = mul(vResultPos, matVP);
        Out[0].vTexUV = float2(0.f, 0.f);
        Out[0].fLife = In[0].fLife / g_fLife;

		vResultPos = vPosition + float4(vUp, 0.f) * fCurWidth;
		Out[1].vPosition = mul(vResultPos, matVP);
        Out[1].vTexUV = float2(1.f, 0.f);
        Out[1].fLife = In[0].fLife / g_fLife;

		vResultPos = vPosition - float4(vUp, 0.f) * fCurWidth;
		Out[2].vPosition = mul(vResultPos, matVP);
        Out[2].vTexUV = float2(1.f, 1.f);
        Out[2].fLife = In[0].fLife / g_fLife;

		vResultPos = vPosition - float4(vUp, 0.f) * fCurWidth;
		Out[3].vPosition = mul(vResultPos, matVP);
        Out[3].vTexUV = float2(0.f, 1.f);
        Out[3].fLife = In[0].fLife / g_fLife;
    }   
    else
    {
        vResultPos = float4(matrix_postion(g_matInfo[In[0].InstanceID - 1]) + vUp * fCurWidth, 1.f);
		Out[0].vPosition = mul(vResultPos, matVP);
		Out[0].vTexUV = float2(0.f, 0.f);
		Out[0].fLife = In[0].fLife / g_fLife;

        vResultPos = vPosition + float4(vUp, 0.f) * fCurWidth;
		Out[1].vPosition = mul(vResultPos, matVP);
		Out[1].vTexUV = float2(1.f, 0.f);
		Out[1].fLife = In[0].fLife / g_fLife;

        vResultPos = vPosition - float4(vUp, 0.f) * fCurWidth;
		Out[2].vPosition = mul(vResultPos, matVP);
		Out[2].vTexUV = float2(1.f, 1.f);
		Out[2].fLife = In[0].fLife / g_fLife;

        vResultPos = float4(matrix_postion(g_matInfo[In[0].InstanceID - 1]) - vUp * fCurWidth, 1.f);
		Out[3].vPosition = mul(vResultPos, matVP);
		Out[3].vTexUV = float2(0.f, 1.f);
		Out[3].fLife = In[0].fLife / g_fLife;
    }

    Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
}

struct PS_IN
{
    float4      vPosition   : SV_Position;
    float2      vTexUV      : TEXCOORD0;
    float       fLife       : TEXCOORD1;
};

struct PS_OUT
{
    float4      vColor      : SV_TARGET0;
};

PS_OUT  PS_MAIN(PS_IN In)
{
    PS_OUT  Out = (PS_OUT)0;

    vector      vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    //if (vColor.a < 0.1f)
    //    discard;

    float   fAlpha = 1.f - (abs(0.5f - In.vTexUV.y) * 2.f);

    vColor = vColor * g_vColor;
    vColor.a = vColor.a * In.fLife;

    if (g_bDistanceAlpha == true)
        vColor.a = vColor.a * fAlpha;

    Out.vColor = vColor;

    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_None);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
    }
}