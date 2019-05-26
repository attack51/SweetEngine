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

	//noise texture resolution 512
	vec2 noiseUV = gl_FragCoord.xy / 512;
	vec2 noiseXY = texture(noiseTex, noiseUV).xy;
	float noise = (noiseXY.x + noiseXY.y) * 0.5 * 0.3;

	vec4 sumColor = vec4(0,0,0,0);

	float shiftScale = 0.7 + noise;
	vec2 shiftUVOffset = shiftScale * inVelocityNDC / SAMPLING_COUNT;

	for(int i=0; i<SAMPLING_COUNT; ++i)
	{
		vec2 shiftUV = screenUV + shiftUVOffset * i;
		sumColor += texture(colorTex, shiftUV);
	}

	outColor = sumColor / SAMPLING_COUNT;
}
