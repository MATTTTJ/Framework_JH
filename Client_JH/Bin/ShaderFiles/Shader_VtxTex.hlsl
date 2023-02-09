
#include "Shader_Client_Defines.h"
#include "Shader_Params.h"

vector			g_vNumColor = (vector)1.f;
vector			g_vSlashColor = (vector)0.f;
float			g_glowStrength = 1.f;
texture2D		g_Texture;
texture2D		g_RingTexture;
texture2D		g_SlashTexture;
texture2D       g_SkillGlowTexture;
texture2D		g_DistortionTexture;
float			g_fTime = 1.f;
texture2D		g_DepthTexture;
texture2D		g_CircleTexture;
texture2D		g_NoiseTexture;

float			g_fFadeAlpha = 0.f;
float			g_fAlpha;
float			g_DisTime;
/* 샘플링 해오는 함수 */
/* dx9 : tex2D(DefaultSampler, In.vTexUV);*/
/* dx11 : g_Texture.Sample(DefaultSampler, In.vTexUV); */

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;


	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vColor : SV_TARGET0;
};

struct PS_OUT_EFFECT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vColor : SV_TARGET0;
	float4		vFlag : SV_TARGET1;
};
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	// Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
	return Out;
}

PS_OUT PS_MAIN_SPHERE_EFFECT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	float4 Noise = g_NoiseTexture.Sample(LinearSampler, In.vTexUV);
	float fWeight;

	if (Noise.r > 0.f)
		fWeight = Noise.r * 0.3f;
	else
		fWeight = 0.f;

	float4 Origin = g_Texture.Sample(LinearSampler, In.vTexUV + fWeight);

	Out.vColor = Origin;

	return Out;
}

PS_OUT PS_MAIN_PORTAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4 vFX_tex = g_DistortionTexture.Sample(LinearSampler, float2(In.vTexUV.x, In.vTexUV.y * g_DisTime));// g_DisTime));
	if (vFX_tex.a == 0.f)
		vFX_tex.r = 0.f;
	// discard;
	float fWeight = vFX_tex.r * 0.1f;

	float4 CircleTex = g_CircleTexture.Sample(LinearSampler, float2(In.vTexUV.x, In.vTexUV.y) + fWeight);

	// float3 center = float3(0.5f, 0.5f, 0);
	// float3 top = float3(0.5f, 0, 0);
	// float3 curUV = float3(In.vTexUV.xy, 0);
	// float angle = 0;daasd
	//
	// float3 centerToTop = top - center;
	// float3 centerToCurUV = curUV - center;
	//
	// centerToTop = normalize(centerToTop);
	// centerToCurUV = normalize(centerToCurUV);
	//
	// angle = acos(dot(centerToTop, centerToCurUV));
	// angle = angle * (180.0f / 3.141592654f); // radian to degree
	//
	// angle = (centerToTop.x * centerToCurUV.x - centerToTop.y * centerToCurUV.x > 0.0f) ? angle : (-angle) + 360.0f;
	//
	// float condition = 360 * g_DisTime;
	//
	// if (angle >= condition)
	// 	CircleTex.a = g_DisTime;

	// CircleTex = CircleTex * g_fAlpha;


	
	float4 vColor = (float4)0.f;
	vColor = g_Texture.Sample(LinearSampler,float2(In.vTexUV.x, g_DisTime));

	if(CircleTex.r > 0.f)
		Out.vColor = saturate(vColor* CircleTex) ;
	else
		Out.vColor = vColor;

	float2		vTexUV;
	vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexUV);;

	float		fOldViewZ = vDepthDesc.y * 300.f;
	float		fViewZ = In.vProjPos.w;

	Out.vColor.a = Out.vColor.a * (saturate(fOldViewZ - fViewZ) * 2.5f);

	// Out.vColor = 
	// g_DistortionTexture
	// Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
	return Out;
}

PS_OUT PS_MAIN_Ring(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	// float4 Glow = g_RingTexture.Sample(LinearSampler, In.vTexUV);

	// Out.vColor = saturate(Out.vColor + Glow);

	// if (Out.vColor.a < 0.1f)
	// 	discard;

	return Out;
}

PS_OUT PS_MAIN_FADE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = g_fFadeAlpha;

	return Out;
}

PS_OUT PS_MAIN_HP_RED(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, float2(In.vTexUV.x + g_fTime, In.vTexUV.y));

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_EFFECT(PS_IN In)
{
	//Soft Effect
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.a == 0.f)
		discard;
	float2		vTexUV;

	vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexUV);

	float		fOldViewZ = vDepthDesc.y * 300.f;
	float		fViewZ = In.vProjPos.w;

	Out.vColor.a = Out.vColor.a * (saturate(fOldViewZ - fViewZ) * 2.5f) ;
	Out.vColor.a = Out.vColor.a * g_fAlpha * 0.1f;
	// if (Out.vColor.a > 0.f)
	// {
	// 	Out.vFlag.r = 0.f;
	// 	Out.vFlag.g = 1.f;
	// 	Out.vFlag.b = 0.f;
	// }
	// else
	// {
	// 	Out.vFlag.r = 0.f;
	// 	Out.vFlag.g = 0.f;
	// 	Out.vFlag.b = 0.f;
	// }


	return Out;
}

PS_OUT PS_MAIN_SLASH(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_SlashTexture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	if (Out.vColor.r < float(0.1f) && Out.vColor.g < float(0.1f) && Out.vColor.b<float(0.1f))
	{
		Out.vColor.rgba = float4(0.f, 0.f, 0.f, 0.9f);
	}
	else
	{
		Out.vColor = g_vSlashColor;
	}

	return Out;
}

PS_OUT PS_MAIN_Number(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.3f)
		discard;

	if (Out.vColor.r < float(0.3f) && Out.vColor.g < float(0.3f) && Out.vColor.b<float(0.3f))
	{
		Out.vColor.rgba = float4(0.f, 0.f, 0.f, 0.9f);
	}
	else
	{
		Out.vColor = g_vNumColor;
	}

	return Out;
}

PS_OUT PS_MAIN_WhiteNum(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.3f)
		discard;

	if (Out.vColor.r < float(0.3f) && Out.vColor.g < float(0.3f) && Out.vColor.b<float(0.3f))
	{
		Out.vColor.rgba = float4(0.f, 0.f, 0.f, 0.9f);
	}
	else
	{
		Out.vColor = float4(1.f,1.f,1.f,1.f);
	}

	return Out;
}

PS_OUT PS_MAIN_GLOW(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4	textureColor;
	float4  glowColor;

	textureColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	glowColor = g_SkillGlowTexture.Sample(LinearSampler, In.vTexUV);
	
	// if (glowColor.r < 0.2f && glowColor.g < 0.2f && glowColor.b < 0.2f)
	// 	glowColor.a = 0.f;
	// glowColor.a = 0.f;

	Out.vColor = saturate(textureColor + (glowColor * g_glowStrength) );
	 
	// if (Out.vColor.a < 0.001f)
	// 	discard;

	return Out;
}


technique11 DefaultTechnique
{
	pass Rect0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI_Alpha2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI_ForSlash3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SLASH();
	}

	pass UI_ForNumber4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Number();
	}

	pass Glow5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GLOW();
	}

	pass Rect6
	{
		SetRasterizerState(RS_None);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend , float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
	}

	pass Rect7
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HP_RED();
	}

	pass FADE8
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FADE();
	}

	pass DangerRing9
	{
		SetRasterizerState(RS_None);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Ring();
	}


	pass Rect10
	{
		SetRasterizerState(RS_CW);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI_ForNumber11
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WhiteNum();
	}

	pass PORTAL12
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_PORTAL();
	}


	pass Rect13
	{
		SetRasterizerState(RS_CW);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPHERE_EFFECT();
	}
}
