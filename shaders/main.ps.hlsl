
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

cbuffer ShaderConstants : register(b0)
{
    float4 g_tint;
    float4 g_sampleParameters;
};

PSOutput Main(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float2 uv = input.uv;
    uv *= g_sampleParameters.zw; // scale 
    uv += g_sampleParameters.xy; // translate
    output.color = Tex.Sample(TexSampler, uv) * g_tint;
    return output;
}