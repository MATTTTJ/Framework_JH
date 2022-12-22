
#include "Shader_Client_Defines.h"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
/* 1. 모델의 전체의 뼈를 받아온다. */
/* 2. 모델 중, 현재 그릴려고 하는 메시에 뼈를 받아온다. */
matrix			g_BoneMatrices[256];

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

// struct GS_IN
// {
// 	float4	vPosition : POSITION;
// 	float3	vNormal : NORMAL;
// 	float  EdgeFlag : PSIZE;
//
// };

// struct GS_OUT
// {
// 	float4 vPosition : VS_POSITION;
// 	float3 vNormal : NORMAL;
// 	float  EdgeFlag : PSIZE;
//
// };
//
// float3 GetNormal(float3 A, float3 B, float C)
// {
// 	float3 AB = normalize(B - A);
// 	float AC = normalize(C - A);
// 	return normalize(cross(AB, AC));
// }
//
// void CreateVertex(inout TriangleStream<GS_OUT> triStream, float3 pos, float3 normal,
// 	float edgeFlag = 0)
// {
// 	//Step 1. Create a GS_DATA object
// 	GS_OUT tempData;
// 	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA
// 	//object).Position(Keep in mind: float3 -> float4)
// 		tempData.vPosition = mul(float4(pos, 1), g_WorldMatrix * g_ViewMatrix * g_ProjMatrix);
// 	//Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA
// 	//object).Normal(Only Rotation, No translation!)
// 		tempData.vNormal = mul(normal, (float3x3)g_WorldMatrix);
// 	//Step 4. Assign texCoord to (GS_DATA object).TexCoord
// 	tempData.EdgeFlag = edgeFlag;
// 	//Step 5. Append (GS_DATA object) to the TriangleStream parameter
// 	//(TriangleStream::Append(...))
// 		triStream.Append(tempData);
// }
//
// [maxvertexcount(12)]
// void GS_MAIN(triangleadj VS_OUT vertices[6], inout TriangleStream<GS_OUT> triStream)
// {
// 	float4 wp1 = vertices[0].vPosition;
// 	float4 wp2 = vertices[2].vPosition;
// 	float4 wp3 = vertices[4].vPosition;
//
// 	float3 faceNormal = GetNormal(wp1, wp2, wp3);
// 	float3 basePoint = float4((wp1 + wp2, wp3));
// 	float3 viewDirection = normalize(basePoint - g_ViewMatrix[3].xyz);
// 	viewDirection = normalize(viewDirection);
//
// 	float dotView = dot(faceNormal, viewDirection);
//
// 	if(dotView < 0)
// 	{
// 		for (uint i = 0; i<6; i += 2)
// 		{
// 			uint iNextTri = (i + 2) % 6;
// 			wp1 = vertices[i].vPosition;
// 			wp2 = vertices[i + 1].vPosition;
// 			wp3 = vertices[iNextTri].vPosition;
// 			faceNormal = GetNormal(wp1, wp2, wp3);
// 			basePoint = float4((wp1 + wp2 + wp3) / 3).xyz;
// 			viewDirection = normalize(basePoint - g_ViewMatrix[3].xyz);
// 			dotView = dot(faceNormal, viewDirection);
// 			if (dotView >= 0)
// 			{
// 				for (int v = 0; v < 2; v++)
// 				{
// 					float3 wsPos = vertices[i].vPosition + v * vertices[i].vNormal * 2;
// 					CreateVertex(triStream, wsPos, vertices[i].vNormal, 3);
// 				}
// 				for (int v = 0; v < 2; v++)
// 				{
// 					float3 wsPos = vertices[iNextTri].vPosition +
// 						v * vertices[iNextTri].vNormal * 2;
// 					CreateVertex(triStream, wsPos, vertices[iNextTri].vNormal, 3);
// 				}
// 				triStream.RestartStrip();
//
// 				CreateVertex(triStream, vertices[0].vPosition, vertices[0].vNormal, 0);
// 				CreateVertex(triStream, vertices[2].vPosition, vertices[2].vNormal, 0);
// 				CreateVertex(triStream, vertices[4].vPosition, vertices[4].vNormal, 0);
// 			}
// 	}
//// }





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
	//
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	if (0.1f > Out.vColor.a)
		discard;
	


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

	if(L < 0.05)
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) * 0.6f ;
	}
	// Out.vColor = float4(L.xxx, 1);
	//
	// if(Out.vColor.rgb / 3.f < 0.99f)
	// {
	// 	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	// }
	// else
	// 	Out.vColor = float4(L.xxx, 1);

	
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
