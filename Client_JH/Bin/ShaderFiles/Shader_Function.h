float2 Get_FlipBookUV(float2 vRectUV, float fCurTime, float fFrameTime, int iRowCnt, int iColCnt)
{
   float fTotalTime = fFrameTime * iRowCnt * iColCnt;
   float fTime = fmod(fCurTime, fTotalTime);
   uint iFrame = uint(fTime / fFrameTime); // idx = y * X + x;

   float fRowSize = 1.f / iRowCnt;
   float fColSize = 1.f / iColCnt;

   float2 vOutUV;
   vOutUV.x = vRectUV.x / iRowCnt + (iFrame % iRowCnt) * fRowSize;
   vOutUV.y = vRectUV.y / iColCnt + (iFrame / iRowCnt) * fColSize;

   return vOutUV;
}

float2 Get_ColorGradientUV(float fGrayColor)
{
   return float2(fGrayColor, fGrayColor);
}

// Tiling : wrap 샘플러 사용시의 텍스쳐 반복 여부 결정값
// offset : uv 이동 값
float2 TilingAndOffset(float2 UV, float2 Tiling, float2 Offset)
{
    return UV * Tiling + Offset;
}

float4 ConstantBiasScale(float4 v4)
{
   return (v4 - 0.5f) * 2.f;
}
float2 ConstantBiasScale(float2 v2)
{
   return (v2 - 0.5f) * 2.f;
}
float ConstantBiasScale(float f)
{
   return (f - 0.5f) * 2.f;
}

float4 FlowMapping(Texture2D flowTexture, Texture2D targetTexture, sampler sample, float2 vUV, float time, float strength, float2 flowTiling, float2 tiling)
{
   float2 flowUV = flowTexture.Sample(sample, TilingAndOffset(vUV, flowTiling, float2(0.f, 0.f))).xy;
   flowUV = ConstantBiasScale(flowUV) * -1.f;

   float2 flowUV1 = flowUV * frac(time) * strength + TilingAndOffset(vUV, tiling, float2(0.f, 0.f));
   float4 flowMap1 = targetTexture.Sample(sample, flowUV1);

   float2 flowUV2 = flowUV * frac(time + 0.5f) * strength + TilingAndOffset(vUV, tiling, float2(0.f, 0.f));
   float4 flowMap2 = targetTexture.Sample(sample, flowUV2);

   float4 flowMap = lerp(flowMap1, flowMap2, abs(ConstantBiasScale(frac(time))));

   return flowMap;
}

// InMinMax : x(min), y(max)
// OutMinMax : x(min), y(max)
float4 Remap(float4 In, float2 InMinMax, float2 OutMinMax)
{
    return OutMinMax.x + (In - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
}
float2 Remap(float2 In, float2 InMinMax, float2 OutMinMax)
{
    return OutMinMax.x + (In - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
}
float Remap(float In, float2 InMinMax, float2 OutMinMax)
{
    return OutMinMax.x + (In - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
}

float4 CalcHDRColor(float4 vColor, float fIntensity)
{
   float4 vSatColor = saturate(vColor);
   float4 vHDRColor = vSatColor * pow(2.f, fIntensity);
   vHDRColor.a = vSatColor.a;

   return vHDRColor;
}

float FresnelEffect(float3 Normal, float3 ViewDir, float Power)
{
    return pow((1.0 - saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}

float3 matrix_right(float4x4 m)
{
   return float3(m[0][0], m[0][1], m[0][2]);
}
float3 matrix_up(float4x4 m)
{
   return float3(m[1][0], m[1][1], m[1][2]);
}
float3 matrix_look(float4x4 m)
{
   return float3(m[2][0], m[2][1], m[2][2]);
}
float3 matrix_postion(float4x4 m)
{
   return float3(m[3][0], m[3][1], m[3][2]);
}