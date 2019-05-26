#version 450
precision lowp int;
precision lowp float;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec2 inUV;

layout (location = 0) out vec2 outUV;

void main()
{
	gl_Position = inPos;
	outUV = inUV;
}
