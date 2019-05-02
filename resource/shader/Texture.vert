#version 450

struct animatedVert
{
	vec4 pos;
	vec4 nor;
};

layout (std140, binding = 0) uniform bufferVals
{
    mat4 mvp;
	vec3 col;
} myBufferVals;

//https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
//std140 : always rounded up to the size of a vec4 (ie: 16-bytes)
//std430 : they are no longer rounded up to a multiple of 16 bytes
//but if you must use struct float3{float x,y,z;} instead of vec3. it sucks!

layout(std140, binding = 1000) readonly buffer animVertSB
{
	animatedVert inVertices[];
} inAnimVertWB;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec3 outColor;

void main()
{
   outUV 		 = inUV;
   outColor		 = myBufferVals.col;

   vec4 inPos	= inAnimVertWB.inVertices[gl_VertexIndex].pos;
   vec4 inNor	= inAnimVertWB.inVertices[gl_VertexIndex].nor;

   gl_Position 	 = inPos * myBufferVals.mvp;
}
