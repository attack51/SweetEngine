#version 450
precision lowp int;
precision lowp float;

layout(binding = 2000) uniform sampler2D colorTex;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(colorTex, uv);
}
