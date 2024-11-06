
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
    float4 g_TintTest;
};

PSOutput Main(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    output.color = Tex.Sample(TexSampler, input.uv) * g_TintTest;
    return output;
}