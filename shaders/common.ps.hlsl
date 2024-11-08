#ifndef COMMON_PS_H
#define COMMON_PS_H


cbuffer GlobalSwitches : register(b0)
{
    int4 switch_reserved;
}

cbuffer GlobalConstants : register(b1)
{
    float4 const_reserved;
};


#endif