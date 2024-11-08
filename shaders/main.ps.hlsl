#include "common.ps.hlsl"

// textures
Texture2D Tex               : register(t0);
SamplerState TexSampler     : register(s0);

struct PSInput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

cbuffer BlitShaderSwitches : register(b2)
{
    uint4 switchesParam1;
}
#define COLORKEYMODE switchesParam1.x


cbuffer BlitShaderConstants : register(b3)
{
    float4 sampleParameters;
    float4 tint;
    uint4 srcKeys;
};

// only works on 8bit color space
unsigned int createARGBdword(int r, int g, int b, int a)
{
    return (((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}


void PerformColorKey(float4 srcColor, float4 dstColor, uint4 colorKeys, int mode)
{
    if (mode & 1) // src color key
    {
        if (createARGBdword(srcColor.r * 255, srcColor.g * 255, srcColor.b * 255, srcColor.a * 255 ) < colorKeys.x)
            discard;
        
        if (createARGBdword(srcColor.r * 255, srcColor.g * 255, srcColor.b * 255, srcColor.a * 255) > colorKeys.y)
            discard;
    }
}

PSOutput Main(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float2 uv = input.uv;
    uv *= sampleParameters.zw; // scale 
    uv += sampleParameters.xy; // translate
    
    float4 col = Tex.Sample(TexSampler, uv) * tint;
    col.a = 1;

    PerformColorKey(col, float4(0, 0, 0, 0), srcKeys, COLORKEYMODE);
    output.color = col;
    return output;
}