#ifndef COMMON_PS_H
#define COMMON_PS_H

SamplerState LinearSampler      : register(s0);
SamplerState PointSampler       : register(s1);

cbuffer GlobalSwitches : register(b0)
{
    int4 switches_1;
}
#define PALETTE_STATE switches_1.x

cbuffer GlobalConstants : register(b1)
{
    float4 const_reserved;
};

cbuffer PaletteState : register(b2)
{
    float4 palleteColor[256];
};

// only works on 8bit color space
unsigned int createARGBdword(int r, int g, int b, int a)
{
    return (((a & 0xf) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xf));
}

unsigned int createARGBshort(int r, int g, int b, int a)
{
    return (((a & 0xf) << 4) + ((r & 0xf) << 3) + ((g & 0xf) << 2) + (b & 0xff));
}

float4 PerformColorPalette(float4 col)
{    
    // 8bit
    if (PALETTE_STATE != 0)
    {
        unsigned int dwordColor = createARGBshort(ceil(col.r * 255), ceil(col.g * 255), ceil(col.b * 255), ceil(col.a * 255));
        if (dwordColor >= 0 && dwordColor <= 255)
        {
            return col;
        }
       
        return col; 
    }
    
    return col;
}

#endif