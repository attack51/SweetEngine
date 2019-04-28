#version 450

layout(binding = 1) uniform sampler2D tex;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 color;

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = texture(tex, uv);
	outColor.rgb *= color;
	//outColor = vec4(1,1,1,1);
	//outColor.gb = vec2(0,0);
}
