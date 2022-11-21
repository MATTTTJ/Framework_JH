matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vCamPosition;

/* ������ */
vector			g_vLightDir;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;


/* �������� */
texture2D		g_DiffuseTexture;
vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


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



/* ���ø� �ؿ��� �Լ� */
/* dx9 : tex2D(DefaultSampler, In.vTexUV);*/
/* dx11 : g_Texture.Sample(DefaultSampler, In.vTexUV); */

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float		fShade : COLORD0;
	float		fSpecular : COLORD1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	vector		vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vector		vWorldNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);

	Out.fShade = saturate(dot(normalize(g_vLightDir) * -1.f, 
		normalize(vWorldNormal)));

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector		vLook = vWorldPosition - g_vCamPosition;

	Out.fSpecular = saturate(dot(normalize(vReflect) * -1.f,
		normalize(vLook)));

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float		fShade : COLORD0;
	float		fSpecular : COLORD1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	/*SV_TARGET0 : ��� ������ ������ �ȼ��̴�. AND 0��° ����Ÿ�ٿ� �׸������� �����̴�. */
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV * 30.f);

	vector		vDiffuse = g_vLightDiffuse * vMtrlDiffuse;

	Out.vColor = vDiffuse * saturate(In.fShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ In.fSpecular * (g_vLightSpecular * g_vMtrlSpecular);
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Terrain
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
