module HelloTexture;

struct Vertex
{
	float2 position;
	float2 uv;
};

public struct VertexOutput
{
    public float4 position : SV_Position;
    public float2 uv;
};

static const Vertex vertices[4] =
{
	{float2(-0.5, -0.5), float2(0.0, 0.0)},
	{float2(-0.5, +0.5), float2(0.0, 1.0)},
	{float2(+0.5, +0.5), float2(1.0, 1.0)},
	{float2(+0.5, -0.5), float2(1.0, 0.0)},
};

static const uint32_t indices[6] = {0, 2, 1, 0, 3, 2};

[vk::binding(0, 0)] Sampler2D texture : register(t0, space0);

[shader("vertex")]
VertexOutput vert(uint32_t vertexID : SV_VertexID)
{
	uint32_t index = indices[vertexID];
	Vertex vertex = vertices[index];

	VertexOutput output;
	output.position = float4(vertex.position, 0.0, 1.0);
	output.uv = vertex.uv;
	return output;
}

[shader("fragment")]
float4 frag(VertexOutput input) : SV_Target
{
	float4 sample = texture.Sample(input.uv);
	return sample;
}