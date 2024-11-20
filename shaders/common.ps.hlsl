#ifndef COMMON_PS_H
#define COMMON_PS_H

SamplerState LinearSampler      : register(s0);
SamplerState PointSampler       : register(s1);
StructuredBuffer<float4>        paletteColors : register(t0);

cbuffer GlobalSwitches : register(b0)
{
    int4 switches_1;
}
#define PALETTE_STATE switches_1.x

cbuffer GlobalConstants : register(b1)
{
    float4 const_reserved;
};

// only works on 8bit color space
unsigned int createARGBdword(int r, int g, int b, int a)
{
    return (((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

unsigned int createARGBshort(int r, int g, int b, int a)
{
    return (((a & 0xf) << 16) + ((r & 0xf) << 8) + ((g & 0xf) << 4) + (b & 0xf));
}

float4 PerformColorPalette(float4 srcColor)
{    
    // 8bit
    if (PALETTE_STATE != 0)
    {
        unsigned int dwordColor = createARGBdword(floor(srcColor.r * 255), floor(srcColor.g * 255), floor(srcColor.b * 255), floor(srcColor.a * 255));

        return float4(paletteColors[dwordColor & 0xff].rgb, 1);
    }
    
    return srcColor;
}

#endif