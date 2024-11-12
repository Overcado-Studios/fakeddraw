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
    return (((a & 0xf) << 4) + ((r & 0xf) << 3) + ((g & 0xf) << 2) + (b & 0xf));
}

float4 PerformColorPalette(float4 col)
{    
    // 8bit
    if (PALETTE_STATE != 0)
    {
        return paletteColors[ceil(saturate(length(col)) * 255)];
    }
    
    return float4(1, 1, 1, 1);
}

#endif