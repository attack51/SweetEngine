#version 450
precision highp int;
precision highp float;

///////////////// uniform buffer ///////////////////////

layout (std140, binding = 0) readonly uniform generalVal
{
    mat4 vp;
	vec4 screenSize;
	float deltaTime;
} inGeneralVal;

layout (std140, binding = 1) readonly uniform animVal
{
	uint vertCount;
	uint prevVertOffset;
	uint curVertOffset;
} inAnimVal;

layout (std140, binding = 2) readonly uniform materialVal
{
    vec3 col;
} inMaterialVal;

///////////////// ~uniform buffer ///////////////////////

struct animatedVert
{
	vec4 pos;
	vec4 nor;
};

vec3 GetVelocityWorld(in vec4 prevPosWorld, in vec4 curPosWorld)
{
   vec3 velocityWorld = (prevPosWorld.xyz - curPosWorld.xyz) / inGeneralVal.deltaTime;
   velocityWorld*=0.03;//magic number for test

   float maxSpeed = 3.0;//magic number for test
   velocityWorld = clamp(velocityWorld, -maxSpeed.xxx, maxSpeed.xxx);

   return velocityWorld;
}

//https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
//std140 : always rounded up to the size of a vec4 (ie: 16-bytes)
//std430 : they are no longer rounded up to a multiple of 16 bytes
//but if you must use struct float3{float x,y,z;} instead of vec3. it sucks!

layout(std140, binding = 1000) readonly buffer animVertSB
{
	animatedVert inVertices[];
} inAnimVertWB;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outExtendPos;
layout (location = 1) out vec2 outVelocityNdc;

void main()
{
   uint instanceId = gl_InstanceIndex;
   uint vertexId = gl_VertexIndex;

   uint crowdVertOffset = instanceId * inAnimVal.vertCount;
   uint vertIndex = crowdVertOffset + vertexId;
   uint prevVertIndex = vertIndex + inAnimVal.prevVertOffset;
   uint curVertIndex = vertIndex + inAnimVal.curVertOffset;

   vec4 curPosWorld	= inAnimVertWB.inVertices[curVertIndex].pos;
   vec4 prevPosWorld = inAnimVertWB.inVertices[prevVertIndex].pos;

   vec3 velocityWorld = GetVelocityWorld(prevPosWorld, curPosWorld);
   
   vec4 extendPosWorld = curPosWorld;
   extendPosWorld.xyz += velocityWorld;

   gl_Position = curPosWorld * inGeneralVal.vp;
   outExtendPos = extendPosWorld * inGeneralVal.vp;

   vec2 posNdc = gl_Position.xy/gl_Position.w;
   vec2 extendNdc = outExtendPos.xy/outExtendPos.w;

   outVelocityNdc = (posNdc - extendNdc) * 0.5;
}
