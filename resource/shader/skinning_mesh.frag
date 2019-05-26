#version 450
precision lowp int;
precision lowp float;

layout(binding = 2000) uniform sampler2D tex;

layout(location = 0) in vec2 inUv;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inVel;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(tex, inUv);
	outColor.rgb *= inColor;
}
