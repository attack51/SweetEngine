#version 450
precision highp int;
precision highp float;

layout (std140, binding = 0) readonly uniform generalVal
{
    mat4 vp;
	vec4 screenSize;
	float deltaTime;
} inGeneralVal;

layout(binding = 2000) uniform sampler2D colorTex;
layout(binding = 2001) uniform sampler2D noiseTex;

layout(location = 0) in vec2 inVelocityNDC;

layout(location = 0) out vec4 outColor;

#define SAMPLING_COUNT 12
void main()
{
	vec2 screenUV = gl_FragCoord.xy * inGeneralVal.screenSize.zw;

	//outColor = texture(colorTex, screenUV + inVelocityNDC);
	
	//noise texture resolution 512
	vec2 noiseUV = gl_FragCoord.xy / 512;
	vec2 noise = texture(noiseTex, noiseUV).xy * 0.3;

	float kernel[SAMPLING_COUNT] =
	{
		0.7,
		0.6,
		0.4,
		0.3,
		0.26,
		0.21,
		0.15,
		0.1,
		0.6,
		0.25,
		0.08,
		0.03
	};

	vec4 sumColor = vec4(0,0,0,0);

	vec2 shiftScale = vec2(0.7, 0.7) + noise;
	vec2 shiftUVOffset = shiftScale * inVelocityNDC / SAMPLING_COUNT;
	float totalWeight = 0;

	for(int i=0; i<SAMPLING_COUNT; ++i)
	{
		float weight = kernel[i];

		vec2 shiftUV = screenUV + shiftUVOffset * i;
		sumColor += texture(colorTex, shiftUV) * weight;

		totalWeight += weight;
	}

	outColor = sumColor / totalWeight;
}
