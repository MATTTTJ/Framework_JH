
#include "Shader_Client_Defines.h"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vCamPosition;		// 빌보드 형식으로 나오게하기위해서

texture2D		g_Texture;
texture2D		g_GlowTexture;



matrix			g_WorldMatrixRotation;
matrix			g_WorldMatrixRotation1;




float				g_iUV_Max_Width_Num;
float				g_iUV_Max_Height_Num;
float				g_iUV_Cur_Width_Num;
float				g_iUV_Cur_Height_Num;

float				G_Power;

int				g_iPlayType = 0;		// 0 :가로로 그린다 ,1: 세로로 그린다 ,2: 가로가 끝나면 세로로 그린다. 3 :


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;		//PSIZE 라는 시멘틱이 존재한다.

	row_major float4x4	Matrix : WORLD;		//Row_major 자료형이 존재
};

struct VS_OUT
{
	float3		vPosition : POSITION;		//실제 픽셀 쉐이더에던질 수 있는 데이터가 아니라서 SV_ 시멘틱을 제외한다.
	float2		vPSize : PSIZE;
};

struct VS_OUT_Lazor
{
	float3		vPosition[2] : POSITION;		//실제 픽셀 쉐이더에던질 수 있는 데이터가 아니라서 SV_ 시멘틱을 제외한다.
	//float3		vPosition2 : NORMAL;
	float2		vPSize : PSIZE;
};


struct GS_IN_Lazor
{
	float3		vPosition[2] : POSITION;
	//float3		vPosition2 : POSITION;
	float2		vPSize : PSIZE;
};


/* 정점이 추가적으로 생성할 수 있는 상황을 준비해둔다.*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector		vPosition = mul(float4(In.vPosition, 1.f), In.Matrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix).xyz;
	Out.vPSize = In.vPSize;

	return Out;
}

/* 정점이 추가적으로 생성할 수 있는 상황을 준비해둔다.*/
VS_OUT_Lazor VS_MAIN_LAZOR(VS_IN In)
{
	VS_OUT_Lazor		Out = (VS_OUT_Lazor)0;


	vector		vPosition = mul(float4(In.vPosition, 1.f), In.Matrix);

	Out.vPosition[0]  = mul(vPosition, g_WorldMatrix).xyz;
	Out.vPosition[1] = mul(vPosition, g_WorldMatrixRotation).xyz;
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
	float4		vPosition : SV_POSITION;		// 실제 지오메트리 쉐이더 연산결과 레스터라이즈를 끝낸데이터를 픽셀 쉐이더에 전달
	float2		vTexUV : TEXCOORD0;
	float2		vTexUVOrigin : TEXCOORD1;
};

[maxvertexcount(6)]		// 정점은 숫자가 관계없으나 20개 이하가 지오메트리의 성능을 발휘하기에 좋다,
void	GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	//  GS_IN In[N] N의 숫자는
	// TOPOLOGY가 라인이면 2개 포인트면 1개 트라이앵글리스트면 3개 

	GS_OUT		Out[4];	// 사각형을 만들거니까 인덱스는 4개필요

						/* 외적결과를 통해 Right,Up,Look 을 구한다 */
	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;		//카메라를 구해온것은 빌보드처럼 만들려고 
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;

	
	Out[0].vTexUVOrigin = float2(0.f, 0.f);
	Out[1].vTexUVOrigin = float2(1.f, 0.f);
	Out[2].vTexUVOrigin = float2(1.f, 1.f);
	Out[3].vTexUVOrigin = float2(0.f, 1.f);




	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(	1.f / g_iUV_Max_Width_Num * (g_iUV_Cur_Width_Num),
			(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num));
	
	vPosition = In[0].vPosition - vRight + vUp;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2( 1.f / g_iUV_Max_Width_Num *( g_iUV_Cur_Width_Num +1.f ),
			(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num));

	vPosition = In[0].vPosition - vRight - vUp;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2( 1.f / g_iUV_Max_Width_Num* (g_iUV_Cur_Width_Num +1.f),
			 (1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num+1.f) );

	vPosition = In[0].vPosition + vRight - vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(1.f / g_iUV_Max_Width_Num * g_iUV_Cur_Width_Num,
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num+1.f));

	
	
	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();		// 이걸 안써주면 라인 스트립처럼그려서 
									//트라이앵글리스트를 쓸려면 한번 끊어줘야한다.

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
}

[maxvertexcount(6)]		// 정점은 숫자가 관계없으나 20개 이하가 지오메트리의 성능을 발휘하기에 좋다,
void	GS_MAIN_Mirror(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	//  GS_IN In[N] N의 숫자는
	// TOPOLOGY가 라인이면 2개 포인트면 1개 트라이앵글리스트면 3개 

	GS_OUT		Out[4];	// 사각형을 만들거니까 인덱스는 4개필요

						/* 외적결과를 통해 Right,Up,Look 을 구한다 */
	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;		//카메라를 구해온것은 빌보드처럼 만들려고 
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;


	Out[0].vTexUVOrigin = float2(0.f, 0.f);
	Out[1].vTexUVOrigin = float2(1.f, 0.f);
	Out[2].vTexUVOrigin = float2(1.f, 1.f);
	Out[3].vTexUVOrigin = float2(0.f, 1.f);




	vPosition = In[0].vPosition + vRight + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[0].vTexUV = float2(g_UV_XCurRatio- g_UV_XSub, g_UV_YCurRatio- g_UV_YSub);
	// 가로개수 + 1  / 가로맥스넘	, 세로개수 +1 / 세로맥스넘

	//Out[0].vTexUV = float2(g_UV_XCurRatio -, g_UV_YCurRatio - g_UV_YSub);
	Out[0].vTexUV = float2(1.f / g_iUV_Max_Width_Num *(g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num));


	vPosition = In[0].vPosition - vRight + vUp;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[1].vTexUV = float2(g_UV_XCurRatio, g_UV_YCurRatio- g_UV_YSub);
	// (가로개수 + 2) / 가로맥스넘	,	 세로개수+ 1 /세로맥스넘 
	Out[1].vTexUV = float2(1.f / g_iUV_Max_Width_Num * (g_iUV_Cur_Width_Num),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num));



	vPosition = In[0].vPosition - vRight - vUp;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[2].vTexUV = float2(g_UV_XCurRatio, g_UV_YCurRatio);
	// (가로개수 + 2) / 가로맥스넘 ,  세로개수+ 2 /세로맥스넘 
	Out[2].vTexUV = float2(1.f / g_iUV_Max_Width_Num * g_iUV_Cur_Width_Num,
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num + 1.f));



	vPosition = In[0].vPosition + vRight - vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[3].vTexUV = float2(g_UV_XCurRatio - g_UV_XSub, g_UV_YCurRatio);
	// (가로개수 + 1) / 가로맥스넘 ,  세로개수+ 2 /세로맥스넘 
	Out[3].vTexUV =float2(1.f / g_iUV_Max_Width_Num* (g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num + 1.f));



	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();		// 이걸 안써주면 라인 스트립처럼그려서 
									//트라이앵글리스트를 쓸려면 한번 끊어줘야한다.

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
}


[maxvertexcount(4)]		// 정점은 숫자가 관계없으나 20개 이하가 지오메트리의 성능을 발휘하기에 좋다,
void	GS_MAIN_Lazor(point GS_IN_Lazor In[1], inout TriangleStream<GS_OUT> Vertices)
{
	GS_OUT		Out[8];	// 사각형을 만들거니까 인덱스는 4개필요



	float3		vLook = g_vCamPosition.xyz - In[0].vPosition[0];
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;
	

	float3		vRight2 =   In[0].vPSize.x * 0.5f;
	float3		vUp2 =   In[0].vPSize.y * 0.5f;

	/*vRight1 = mul(vRight1, g_WorldMatrixRotation).xyz;
	vUp1 = mul(vUp1, g_WorldMatrixRotation).xyz;
*/

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;


	Out[0].vTexUVOrigin = float2(0.f, 0.f);
	Out[1].vTexUVOrigin = float2(1.f, 0.f);
	Out[2].vTexUVOrigin = float2(1.f, 1.f);
	Out[3].vTexUVOrigin = float2(0.f, 1.f);

	Out[4].vTexUVOrigin = float2(0.f, 0.f);
	Out[5].vTexUVOrigin = float2(1.f, 0.f);
	Out[6].vTexUVOrigin = float2(1.f, 1.f);
	Out[7].vTexUVOrigin = float2(0.f, 1.f);

	//뾰족하게 나오고싶다면 vRight *2 를 하면 됀다.
	//vPosition = In[0].vPosition[0] + vRight + vUp;
	//Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[0].vTexUV = float2(1.f / g_iUV_Max_Width_Num * (g_iUV_Cur_Width_Num),
	//	(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num));

	//vPosition = In[0].vPosition[0] - vRight + vUp;
	//Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[1].vTexUV = float2(1.f / g_iUV_Max_Width_Num *(g_iUV_Cur_Width_Num + 1.f),
	//	(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num));

	//vPosition = In[0].vPosition[0] - vRight - vUp;
	//Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[2].vTexUV = float2(1.f / g_iUV_Max_Width_Num* (g_iUV_Cur_Width_Num + 1.f),
	//	(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num + 1.f));

	//vPosition = In[0].vPosition[0] + vRight - vUp;
	//Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	//Out[3].vTexUV = float2(1.f / g_iUV_Max_Width_Num * g_iUV_Cur_Width_Num,
	//	(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num + 1.f));

	///* 2  */
	vPosition = In[0].vPosition[0] + vRight2 + vUp2;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(1.f / g_iUV_Max_Width_Num * (g_iUV_Cur_Width_Num),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num));

	vPosition = In[0].vPosition[0] - vRight2 + vUp2;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f / g_iUV_Max_Width_Num *(g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num));

	vPosition = In[0].vPosition[0] - vRight2 - vUp2;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f / g_iUV_Max_Width_Num* (g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num + 1.f));

	vPosition = In[0].vPosition[0] + vRight2 - vUp2;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(1.f / g_iUV_Max_Width_Num * g_iUV_Cur_Width_Num,
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num + 1.f));
	///* ~2 */

	/*1*/
	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();		
									
	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
	/*~1*/

	///*///*2*/
	/*Vertices.Append(Out[4]);
	Vertices.Append(Out[5]);
	Vertices.Append(Out[6]);
	Vertices.RestartStrip();									
	
	Vertices.Append(Out[4]);
	Vertices.Append(Out[6]);
	Vertices.Append(Out[7]);
	Vertices.RestartStrip();*/
	///*~2*/*/

}


[maxvertexcount(6)]		// 정점은 숫자가 관계없으나 20개 이하가 지오메트리의 성능을 발휘하기에 좋다,
void	GS_MAIN_Test(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
	//  GS_IN In[N] N의 숫자는
	// TOPOLOGY가 라인이면 2개 포인트면 1개 트라이앵글리스트면 3개 

	GS_OUT		Out[4];	// 사각형을 만들거니까 인덱스는 4개필요

						/* 외적결과를 통해 Right,Up,Look 을 구한다 */
	float3		vLook = g_vCamPosition.xyz - In[0].vPosition;		//카메라를 구해온것은 빌보드처럼 만들려고 
	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3		vPosition;


	Out[0].vTexUVOrigin = float2(0.f, 0.f);
	Out[1].vTexUVOrigin = float2(1.f, 0.f);
	Out[2].vTexUVOrigin = float2(1.f, 1.f);
	Out[3].vTexUVOrigin = float2(0.f, 1.f);




	vPosition = In[0].vPosition + (vRight*2) + vUp;
	Out[0].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[0].vTexUV = float2(1.f / g_iUV_Max_Width_Num * (g_iUV_Cur_Width_Num),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num));

	vPosition = In[0].vPosition - (vRight * 2) + vUp;
	Out[1].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[1].vTexUV = float2(1.f / g_iUV_Max_Width_Num *(g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num));

	vPosition = In[0].vPosition - (vRight * 2) - vUp;
	Out[2].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[2].vTexUV = float2(1.f / g_iUV_Max_Width_Num* (g_iUV_Cur_Width_Num + 1.f),
		(1.f) / g_iUV_Max_Height_Num * (g_iUV_Cur_Height_Num + 1.f));

	vPosition = In[0].vPosition + (vRight * 2) - vUp;
	Out[3].vPosition = mul(vector(vPosition, 1.f), matVP);
	Out[3].vTexUV = float2(1.f / g_iUV_Max_Width_Num * g_iUV_Cur_Width_Num,
		(1.f) / g_iUV_Max_Height_Num* (g_iUV_Cur_Height_Num + 1.f));



	Vertices.Append(Out[0]);
	Vertices.Append(Out[1]);
	Vertices.Append(Out[2]);
	Vertices.RestartStrip();		// 이걸 안써주면 라인 스트립처럼그려서 
									//트라이앵글리스트를 쓸려면 한번 끊어줘야한다.

	Vertices.Append(Out[0]);
	Vertices.Append(Out[2]);
	Vertices.Append(Out[3]);
	Vertices.RestartStrip();
}



/*  지오메트리 쉐이더가 하는 일 :
w나누기 -> 뷰포트 변환 -> 레스터 라이즈까지 끝낸 형태*/

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vTexUVOrigin : TEXCOORD1;
};

struct PS_OUT
{
	/*SV_TARGET0 : 모든 정보가 결정된 픽셀이다. AND 0번째 렌더타겟에 그리기위한 색상이다. */
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(PointSampler, In.vTexUV);
	//Out.vColor.rgb = float3(1.f, 0.f, 0.f);
	
	if (Out.vColor.a < 0.2f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_Glow(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//Out.vColor = g_Texture.Sample(PointSampler, In.vTexUV);
	//Out.vColor.rgb = float3(1.f, 0.f, 0.f);

	


	float4	GlowColor, TexturColor;
	TexturColor = g_Texture.Sample(LinearSampler, In.vTexUV);
	GlowColor = g_GlowTexture.Sample(LinearSampler, In.vTexUV);
	
	
	Out.vColor = saturate(TexturColor   + (GlowColor* G_Power));
		
	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}



technique11 DefaultTechnique
{
	pass Rect
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

	pass Glow
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Glow();
	}

	pass Mirror
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN_Mirror();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	

	pass Test
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN_Test();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}



}
