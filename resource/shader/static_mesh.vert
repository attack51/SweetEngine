#version 450

layout (std140, binding = 0) uniform bufferVals
{
    mat4 mvp;
	vec3 col;
} myBufferVals;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inNor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec3 outColor;

void main()
{
   outUV 		 = inUV;
   outColor		 = myBufferVals.col;

   gl_Position 	 = vec4(inPos, 1) * myBufferVals.mvp;
}
