
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ProjMatrixInv, g_ViewMatrixInv;
matrix			g_matLightView, g_matLightProj;



vector			g_vLightDir;
vector			g_vLightPos;
float			g_fLightRange;
float			g_fFar = 300.f;

float			g_iWinCX;
float			g_iWinCY;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vCamPosition;

vector			g_vMtrlAmbient = (vector)1.f;
vector			g_vMtrlSpecular = (vector)1.f;
float			g_fPower = 30.f;

texture2D		g_Texture; /* 디버그용텍스쳐*/
texture2D		g_NormalTexture_Deferred;
texture2D		g_DepthTexture_Deferred;
texture2D		g_SpecularMapTexture_Deferred;
texture2D		g_ShadowDynamicDepthTexture;
texture2D		g_ShadowStaticDepthTexture;
texture2D		g_EffectTexture;
texture2D		g_BloomTexture;
texture2D		g_FlagTexture;
texture2D		g_BlurTexture;
texture2D		g_HorizontalBlurTexture;
texture2D		g_VerticalBlurTexture;
texture2D		g_DiffuseTexture_Deferred;
texture2D		g_ShadeTexture_Deferred;
texture2D		g_SpecularTexture_Deferred;
texture2D		g_OutlineTexture;
texture2D		g_OutlineFlagTexture;

sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state
{
	filter = min_mag_mip_Point;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	return Out;
}


struct PS_OUT_LIGHT
{
	float4		vShade : SV_TARGET0;
	float4		vSpecular : SV_TARGET1;
};


PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc =	g_NormalTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector		vDepthDesc =	g_DepthTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	float		fViewZ =		vDepthDesc.y * 300.f;

	/* 0 ~ 1 => -1 ~ 1 */
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	/* 화면에 그려지고 있는 픽셀들의 투영스페이스 상의 위치. */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / z */
	vector		vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x; /* 0 ~ 1 */
	vWorldPos.w = 1.0f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬  */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
	vector		vLook = vWorldPos - g_vCamPosition;
	// vector		Specular = g_SpecularMapTexture_Deferred.Sample(LinearSampler, In.vTexUV);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);

	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_FOV(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	/* 방향성광원의 정보와 노멀 타겟에 담겨있는 노멀과의 빛연산을 수행한다. */
	vector			vNormalDesc = g_NormalTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * 300.f;
	/* 0 -> -1, 1 -> 1*/
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector			vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */
	/* 뷰스페이스 상 * 투영행렬 / w 까지 위치를 구한다. */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	/* 뷰스페이스 상 * 투영행렬까지 곱해놓은 위치를 구한다. */
	vWorldPos *= fViewZ;

	/* 뷰스페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);


	/* 월드페이스 상  위치를 구한다. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLightDir = vWorldPos - g_vLightPos;

	float			fDistance = length(vLightDir);

	float			fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

	fAtt = pow(fAtt, 0.8f);


	//그림자
	vector vLightPos = mul(vWorldPos, g_matLightView);

	vector		vUVPos = mul(vLightPos, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector	vShadowDynamicDepth = g_ShadowDynamicDepthTexture.Sample(LinearSampler, vNewUV);

	if (vUVPos.z - 0.01f > vShadowDynamicDepth.r)
		fAtt = 0.f;

	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * fAtt + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(g_vLightDir), vNormal);
	vector			vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), g_fPower) * fAtt;
	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	float		fViewZ = vDepthDesc.y * 300.f;

	/* 0 ~ 1 => -1 ~ 1 */
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	/* 화면에 그려지고 있는 픽셀들의 투영스페이스 상의 위치. */
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / z */
	vector		vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x; /* 0 ~ 1 */
	vWorldPos.w = 1.0f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬  */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;

	float		fDistance = length(vLightDir);

	float		fAtt = max((g_fLightRange - fDistance), 0.f) / g_fLightRange;

	Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector		vLook = normalize(vWorldPos - g_vCamPosition);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f) * fAtt;
	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector		vSpecular = g_SpecularTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	vector		vFlag = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBloom = g_BloomTexture.Sample(LinearSampler, In.vTexUV);
	vector		vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexUV);
	vector		vOutline = g_OutlineTexture.Sample(LinearSampler, In.vTexUV);

	if (vBlur.a >= 0.05f)
	{
		//vBlur.rgb *= 3.f;
		Out.vColor = vector(vBlur.rgb * vBlur.a + vDiffuse.rgb * (1.f - vBlur.a), 1.f) * vShade + vSpecular;

		//글로우
		if (vFlag.b == 1.f)
		{
			if (vEffect.a > 0.1f)
			{
				Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f) * vShade + vSpecular;
			}
		}
	}
	else
	{
		//이펙트 효과 없을 때
		if (vEffect.a > 0.1f && vFlag.r == 0.f && vFlag.g == 0.f && vFlag.b == 0.f)
		{
			vDiffuse = vDiffuse * vShade + vSpecular;
			Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
		}
		else
		{
			Out.vColor = vDiffuse * vShade + vSpecular;
		}

	}
	//블룸
	if (vFlag.g >= 0.1f)
	{
		vEffect.rgb = pow(pow(abs(vBloom.rgb), 2.2f) + pow(abs(vEffect.rgb), 2.2f), 1.f / 2.2f);
		Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
	}
	//
	if (vOutline.a == 1.f)
		Out.vColor.rgb = vOutline.rgb;

	if (Out.vColor.a == 0.f)
		discard;

	// if (vBlur.a >= 0.05f)
	// {
	// 	//vBlur.rgb *= 3.f;
	// 	Out.vColor = vector(vBlur.rgb * vBlur.a + vDiffuse.rgb * (1.f - vBlur.a), 1.f) * vShade + vSpecular;
	//
	// // 	//글로우
	// 	if (vFlag.b == 1.f)
	// 	{
	// 		if (vEffect.a > 0.1f)
	// 		{
	// 			Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f) * vShade + vSpecular;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	//이펙트 효과 없을 때
	// 	if (vEffect.a > 0.1f && vFlag.r == 0.f && vFlag.g == 0.f && vFlag.b == 0.f)
	// 	{
	// 		vDiffuse = vDiffuse * vShade + vSpecular;
	// 		Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
	// 	}
	// 	else
	// 	{
	// 		Out.vColor = vDiffuse * vShade + vSpecular;
	// 	}
	// //
	// }
	// // //블룸
	// if (vFlag.g >= 0.1f)
	// {
	// 	vEffect.rgb = pow(pow(abs(vBloom.rgb), 2.2f) + pow(abs(vEffect.rgb), 2.2f), 1.f / 2.2f);
	// 	Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
	// }
	//
	//
	// if (Out.vColor.a == 0.f)
	// 	discard;
	//
	// return Out;
	//
	//
	// if (vBlur.a >= 0.05f)
	// {
	// 	//vBlur.rgb *= 3.f;
	// 	Out.vColor = vector(vBlur.rgb * vBlur.a + vDiffuse.rgb * (1.f - vBlur.a), 1.f) * vShade + vSpecular;
	//
	// 	//글로우
	// 	if (vFlag.b == 1.f)
	// 	{
	// 		if (vEffect.a > 0.1f)
	// 		{
	// 			Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f) * vShade + vSpecular;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	//이펙트 효과 없을 때
	// 	if (vEffect.a > 0.1f && vFlag.r == 0.f && vFlag.g == 0.f && vFlag.b == 0.f)
	// 	{
	// 		vDiffuse = vDiffuse * vShade + vSpecular;
	// 		Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
	// 	}
	// 	else
	// 	{
	// 		Out.vColor = vDiffuse * vShade + vSpecular; // 원래 쓰던거
	// 	}
	//
	// }
	// //블룸
	// if (vFlag.g >= 0.1f)
	// {
	// 	vEffect.rgb = pow(pow(abs(vBloom.rgb), 2.2f) + pow(abs(vEffect.rgb), 2.2f), 1.f / 2.2f);
	// 	Out.vColor = vector(vEffect.rgb * vEffect.a + vDiffuse.rgb * (1.f - vEffect.a), 1.f);
	// }
	//
	// // if (vOutline.a == 1.f)
	// // 	Out.vColor.rgb = vOutline.rgb;
	//
	// if (Out.vColor.a == 0.f)
	// 	discard;

	return Out;
}

struct VS_OUT_BLUR
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		texCoord1 : TEXCOORD1;
	float2		texCoord2 : TEXCOORD2;
	float2		texCoord3 : TEXCOORD3;
	float2		texCoord4 : TEXCOORD4;
	float2		texCoord5 : TEXCOORD5;
	float2		texCoord6 : TEXCOORD6;
	float2		texCoord7 : TEXCOORD7;
	float2		texCoord8 : TEXCOORD8;
	float2		texCoord9 : TEXCOORD9;
};

VS_OUT_BLUR VS_HORIZONTALBLUR(VS_IN In)
{
	VS_OUT_BLUR		Out = (VS_OUT_BLUR)0;
	float texelSize;

	matrix			matWV, matWVP;

	In.vPosition *= 0.5f;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	texelSize = 1.f / 4000.f;

	Out.texCoord1 = In.vTexUV + float2(texelSize * -4.0f, 0.0f);
	Out.texCoord2 = In.vTexUV + float2(texelSize * -3.0f, 0.0f);
	Out.texCoord3 = In.vTexUV + float2(texelSize * -2.0f, 0.0f);
	Out.texCoord4 = In.vTexUV + float2(texelSize * -1.0f, 0.0f);
	Out.texCoord5 = In.vTexUV + float2(texelSize *  0.0f, 0.0f);
	Out.texCoord6 = In.vTexUV + float2(texelSize *  1.0f, 0.0f);
	Out.texCoord7 = In.vTexUV + float2(texelSize *  2.0f, 0.0f);
	Out.texCoord8 = In.vTexUV + float2(texelSize *  3.0f, 0.0f);
	Out.texCoord9 = In.vTexUV + float2(texelSize *  4.0f, 0.0f);

	return Out;
}

VS_OUT_BLUR VS_VERTICALBLUR(VS_IN In)
{
	VS_OUT_BLUR		Out = (VS_OUT_BLUR)0;
	float texelSize;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	texelSize = 1.f / 1800.f;

	Out.texCoord1 = In.vTexUV + float2(0.f, texelSize * -4.0f);
	Out.texCoord2 = In.vTexUV + float2(0.f, texelSize * -3.0f);
	Out.texCoord3 = In.vTexUV + float2(0.f, texelSize * -2.0f);
	Out.texCoord4 = In.vTexUV + float2(0.f, texelSize * -1.0f);
	Out.texCoord5 = In.vTexUV + float2(0.f, texelSize *  0.0f);
	Out.texCoord6 = In.vTexUV + float2(0.f, texelSize *  1.0f);
	Out.texCoord7 = In.vTexUV + float2(0.f, texelSize *  2.0f);
	Out.texCoord8 = In.vTexUV + float2(0.f, texelSize *  3.0f);
	Out.texCoord9 = In.vTexUV + float2(0.f, texelSize *  4.0f);

	return Out;
}

VS_OUT VS_DOUBLE(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	In.vPosition *= 2.f;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN_BLUR
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		texCoord1 : TEXCOORD1;
	float2		texCoord2 : TEXCOORD2;
	float2		texCoord3 : TEXCOORD3;
	float2		texCoord4 : TEXCOORD4;
	float2		texCoord5 : TEXCOORD5;
	float2		texCoord6 : TEXCOORD6;
	float2		texCoord7 : TEXCOORD7;
	float2		texCoord8 : TEXCOORD8;
	float2		texCoord9 : TEXCOORD9;
};

PS_OUT PS_HORIZONTALBLUR(PS_IN_BLUR In)
{
	PS_OUT			Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	weight0 = 1.0f;
	weight1 = 0.75f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;
	// weight0 = 0.199471f;
	// weight1 = 0.176033f;
	// weight2 = 0.120985f;
	// weight3 = 0.064759f;
	// weight4 = 0.025995f;

	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	Out.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord1) * weight4;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord2) * weight3;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord3) * weight2;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord4) * weight1;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord5) * weight0;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord6) * weight1;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord7) * weight2;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord8) * weight3;
	Out.vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, In.texCoord9) * weight4;

	//Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_VERTICALBLUR(PS_IN_BLUR In)
{
	PS_OUT			Out = (PS_OUT)0;

	float weight0, weight1, weight2, weight3, weight4;
	weight0 = 1.0f;
	weight1 = 0.75f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;
	// weight0 = 0.199471f;
	// weight1 = 0.176033f;
	// weight2 = 0.120985f;
	// weight3 = 0.064759f;
	// weight4 = 0.025995f;


	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	Out.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord1) * weight4;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord2) * weight3;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord3) * weight2;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord4) * weight1;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord5) * weight0;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord6) * weight1;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord7) * weight2;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord8) * weight3;
	Out.vColor += g_HorizontalBlurTexture.Sample(LinearSampler, In.texCoord9) * weight4;

	//Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_BLUR(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_VerticalBlurTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

float4 vGaussFilter[7] =
{
	0.f, -3.f, 0.f, 1.f / 64.f,
	0.f, -2.f, 0.f, 6.f / 64.f,
	0.f, -1.f, 0.f, 15.f / 64.f,
	0.f, 0.f, 0.f, 20.f / 64.f,
	0.f, 1.f, 0.f, 15.f / 64.f,
	0.f, 2.f, 0.f, 6.f / 64.f,
	0.f, 3.f, 0.f, 1.f / 64.f,
};

PS_OUT PS_BLOOMFLAG(PS_IN In)
{
	// PS_OUT			Out = (PS_OUT)0;
	//
	// // 블룸을 블러한 텍스쳐, 블러 안한 블룸텍스쳐, HDR컬러는 적용시킬 장면에 블룸을 추출한 텍스쳐 
	//
	// vector vFlag = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
	// if (vFlag.r == 1.f)
	// {
	// 	Out.vColor = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	// }
	//
	// else if (vFlag.g == 1.f)
	// {
	// 	vector vHDRColor = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	// 	vector vBloomColor = g_BlurTexture.Sample(LinearSampler, In.vTexUV);
	// 	vector vBloomOrigin = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	//
	// 	float fBrightness = dot(vHDRColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	//
	// 	if (fBrightness > 0.99f)
	// 	{
	// 		vBloomOrigin = vector(vHDRColor.rgb, 1.f);
	// 	}
	//
	// 	float4 vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOrigin), 2.2f), 1.f / 2.2f);
	//
	// 	float4 vOut = (vHDRColor);
	//
	// 	vOut = pow(abs(vOut), 2.2f);
	// 	vBloom = pow(abs(vBloom), 2.2f);
	// 	vOut += vBloom;
	// 	Out.vColor = pow(abs(vOut), 1 / 2.2f);;
	// }

	// 위에가 원래
	PS_OUT			Out = (PS_OUT)0;

	vector vFlag = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
	vector vColor;
	if (vFlag.g != 1.f)
		discard;
	else
		vColor = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);

	// float fBrightness = dot(vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	// if (fBrightness > 0.99f)
	// {
	// 	Out.vColor = vector(vColor.rgb * 4.f, 1.f);
	// }
	float4 vSatColor = saturate(vColor);
	float4 vHDRColor = vSatColor * pow(2.f, 0.6f);
	vHDRColor.a = vSatColor.a;
	Out.vColor = vHDRColor;
	Out.vColor.a = vColor.a;

	//float texelSize = 1.f / g_iWinCY;
	//vector vColor;
	//
	//for (int i = 0; i < 7; ++i)
	//{
	//	vColor += g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x, In.vTexUV.y + vGaussFilter[i].y * texelSize)) * vGaussFilter[i].w;
	//}
	//vColor = vColor * 4.f;
	
	// else
	// {
	// 	Out.vColor = vFlag;
	// }

	return Out;

	

	// float texelSize = 1.f / g_iWinCY;
	// vector vColor;
	//
	// for (int i = 0; i < 7; ++i)
	// {
	// 	vColor += g_DiffuseTexture_Deferred.Sample(LinearSampler, float2(In.vTexUV.x, In.vTexUV.y + vGaussFilter[i].y * texelSize)) * vGaussFilter[i].w;
	// }
	// vColor = vColor * 4.f;

	/////////
	// vector vDepthEmi = g_DepthTexture_Deferred.Sample(LinearSampler, In.vTexUV);
	//
	// float4 vSatColor = saturate(vColor);
	// float4 vHDRColor = vSatColor * pow(2.f, vDepthEmi.b);
	// vHDRColor.a = vSatColor.a;
	// Out.vColor = vHDRColor;
	// return Out;
	//////////
	
	
	// return Out;
}

PS_OUT PS_BLURFLAG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vFlag = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
	
	if (vFlag.r != 1.f)
	{
		discard;
	}
	else
	{
		Out.vColor = g_DiffuseTexture_Deferred.Sample(LinearSampler, In.vTexUV);

		// if (vFlag.b == 1.f)
		// {
		// 	if (Out.vColor.a > 0.1f)
		// 	{
		// 		Out.vColor = vector(Out.vColor.rgb * Out.vColor.a + Out.vColor.rgb * (1.f - Out.vColor.a), 1.f);
		// 	}
		// }
	}

	// Out.vColor = g_FlagTexture.Sample(LinearSampler, In.vTexUV);
	//
	// if (Out.vColor.r != 1.f)
	// 	discard;

	return Out;
}

float mask[9] =
{
	-1, -1, -1,
	-1, 8, -1,
	-1, -1, -1
}; //라플라스 필터

float coord[3] = { -1.5f, 0.f, 1.5f };

PS_OUT PS_OUTLINE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vFlag = g_OutlineFlagTexture.Sample(LinearSampler, In.vTexUV);
	float fFlag;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fFlag += g_OutlineFlagTexture.Sample(LinearSampler, In.vTexUV + float2(coord[j] / 2540, coord[i] / 1369)).r;
		}
	}
	fFlag /= 9.f;
	
	if (vFlag.r > 0.f && vFlag.r != fFlag)
	{
		if (vFlag.r == 1.f)
		{
			Out.vColor = vector(0.f, 0.f, 0.f, 1.f);
		}
		else
			Out.vColor = vector(vFlag.rgb, 1.f);
	}
	else
	{
		Out.vColor.a = 0.f;
		discard;
	}

	// if (Out.vColor.a == 0.f)
	// 	discard;

	// for (int i = 0; i < 3; i++)
	// {
	// 	for (int j = 0; j < 3; j++)
	// 	{
	// 		fFlag += mask[i * 3 + j] * g_FlagTexture.Sample(LinearSampler, In.vTexUV + float2(coord[j] / g_iWinCX, coord[i] / g_iWinCY));
	// 	}
	// }
	// float fGray = 1 - (vFlag.r * 0.3 + vFlag.g * 0.59 + vFlag.b * 0.11);
	//
	// Out.vColor = vector(fGray, fGray, fGray, 1.f);

	return Out;
}








RasterizerState RS_Default
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};


RasterizerState RS_CW
{
	CullMode = Front;
	FrontCounterClockwise = false;
};

DepthStencilState DS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DS_ZEnable_ZWriteEnable_FALSE
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

BlendState BS_Default
{
	BlendEnable[0] = false;


	//bool BlendEnable;
	//D3D11_BLEND SrcBlend;
	//D3D11_BLEND DestBlend;
	//D3D11_BLEND_OP BlendOp;
	//D3D11_BLEND SrcBlendAlpha;
	//D3D11_BLEND DestBlendAlpha;
	//D3D11_BLEND_OP BlendOpAlpha;
	//UINT8 RenderTargetWriteMask;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = Add;
};

BlendState BS_One
{
	BlendEnable[0] = true;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = Add;
};


technique11 DefaultTechnique
{
	pass Debug0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_One, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Light_Point2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_One, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	pass Blend3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	pass Blend4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	pass HorizontalBlur5
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_HORIZONTALBLUR();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HORIZONTALBLUR();
	}

	pass VerticalBlur6
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_VERTICALBLUR();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICALBLUR();
	}

	pass Blur7
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOUBLE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR();
	}

	pass BloomFlag8
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOOMFLAG();
	}

	pass BlurFlag9
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLURFLAG();
	}

	pass Outline10
	{
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE();
	}

	pass Light_Fov11
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_ZEnable_ZWriteEnable_FALSE, 0);
		SetBlendState(BS_One, float4(0.0f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_FOV();
	}
}
