#version 450
//precision highp int;
//precision highp float;

///////////////// uniform buffer ///////////////////////

layout (std140, binding = 0) uniform generalVal
{
    mat4 vp;
} inGeneralVal;

layout (std140, binding = 1) uniform animVal
{
	uint vertCount;
} inAnimVal;

layout (std140, binding = 2) uniform materialVal
{
    vec3 col;
} inMaterialVal;

///////////////// ~uniform buffer ///////////////////////

struct animatedVert
{
	vec4 pos;
	vec4 nor;
};

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
   outColor		 = inMaterialVal.col;

   uint instanceId = gl_InstanceIndex;
   uint vertexId = gl_VertexIndex;

   uint crowdVertOffset = instanceId * inAnimVal.vertCount;
   uint vertIndex = crowdVertOffset + vertexId;

   vec4 inPos	= inAnimVertWB.inVertices[vertIndex].pos;
   vec4 inNor	= inAnimVertWB.inVertices[vertIndex].nor;

   gl_Position 	 = inPos * inGeneralVal.vp;
}
