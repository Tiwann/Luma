module Renderer2D;
import Common;

enum QuadMode : uint32_t
{
	Quad,
    Ellipse,
    Text,
    Sprite
}

struct VertexInput
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
	QuadMode mode : MODE;
	uint32_t textureId : TEXID;
}

struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 texCoord;
	float4 color;
	nointerpolation QuadMode mode;
	nointerpolation uint32_t textureId;
}

[vk::push_constant] ConstantBuffer<float4x4> modelViewProj;
[vk::binding(0, 0)] SamplerState sampler;
[vk::binding(1, 0)] Texture2D textures[];


Texture2D getTexture(uint32_t index)
{
	return textures[NonUniformResourceIndex(index)];
}

[shader("vertex")]
VertexOutput vert(VertexInput input)
{
	VertexOutput output;
	output.position = modelViewProj * float4(input.position, 0.0f, 1.0);
	output.texCoord = input.texCoord;
	output.color = input.color;
	output.mode = input.mode;
	output.textureId = input.textureId;
	return output;
}

float ellipseMask(float2 uv)
{
	// distance is using sqrt. optimize by using squared distance.
	// achieving that using dot(x, x)
	float2 center = (uv - float2(0.5, 0.5)) * 2.0;
	float length = dot(center, center);
	float mask = 1.0 - smoothstep(0.993, 0.999, length);
	return saturate(mask);
}

float4 drawEllipse(float2 uv, float4 color)
{
	return color * ellipseMask(uv);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float median(float3 val)
{
	return median(val.r, val.g, val.b);
}

float2 textureSize(Texture2D texture, uint32_t lod)
{
	float width = 0, height = 0, numLods = 0;
	texture.GetDimensions(lod, width, height, numLods);
	return float2(width, height);
}

float screenPxRange(Texture2D texture, float pixelRange, float2 uv)
{
    float2 unitRange = float2(pixelRange) / textureSize(texture, 0);
    float2 screenTexSize = float2(1.0) / fwidth(uv);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float4 drawCharacter(Texture2D texture, float2 uv, float4 color)
{
	float3 msd = texture.Sample(sampler, uv).rgb;
    float sd = median(msd);
    float screenPxDistance = screenPxRange(texture, 4, uv) * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    return lerp(float4(0, 0, 0, 0), color, opacity);
}

float4 drawSprite(Texture2D texture, float2 uv, float4 color)
{
	float4 sample = texture.Sample(sampler, uv);
	return sample * color;
}

[shader("fragment")]
float4 frag(VertexOutput input) : SV_TARGET
{
	switch(input.mode)
	{
	case QuadMode::Quad: return input.color;
	case QuadMode::Ellipse: return drawEllipse(input.texCoord, input.color);
	case QuadMode::Text: return drawCharacter(getTexture(input.textureId), input.texCoord, input.color);
	case QuadMode::Sprite: return drawSprite(getTexture(input.textureId), input.texCoord, input.color);
	}

	return float4(1.0, 0.03, 1.0, 1.0);
}