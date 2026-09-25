module Skybox;

struct VertexOutput
{
    float4 position : SV_Position;
    float2 ndc;
}

static const float2 kVertexPositions[4] =
{
    float2(-1.0, +1.0),
    float2(+1.0, +1.0),
    float2(+1.0, -1.0),
    float2(-1.0, -1.0),
};

static const uint32_t kIndices[6] = { 0, 2, 1, 0, 3, 2 };

[shader("vertex")]
VertexOutput vert(uint32_t vertexID : SV_VertexID)
{
    uint32_t index = kIndices[vertexID];

    VertexOutput output;
    output.position = float4(kVertexPositions[index], 1.0, 1.0);
    output.ndc = kVertexPositions[index];
    return output;
}

struct PushConstants
{
    float4x4 invViewProj;
    float3 cameraPos;
};

[[vk::binding(0, 0)]] Sampler2D hdriTexture;
[[vk::push_constant]] uniform PushConstants constants;

static const float2 kInvAtan = float2(0.1591, 0.3183);

float2 getEquirectTexCoords(float3 dir)
{
    float2 uv = float2(atan2(dir.z, dir.x), asin(dir.y));
    uv *= kInvAtan;
    uv += 0.5;
    return uv;
}

[shader("fragment")]
float4 frag(VertexOutput input) : SV_Target
{
    float2 ndc = float2(input.ndc.x, -input.ndc.y);
    float4 world = mul(constants.invViewProj, float4(ndc, 1.0, 1.0));
    world /= world.w;

    float3 dir = normalize(world.xyz - constants.cameraPos);
    float2 uv = getEquirectTexCoords(dir);
    return float4(hdriTexture.Sample(uv).rgb, 1.0);
}
