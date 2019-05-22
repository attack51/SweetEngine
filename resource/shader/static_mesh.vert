#version 450

//binding 0 is generalVal. no use it for this shader

layout (std140, binding = 1) uniform staticVal
{
    mat4 wvp;
} inStaticVal;

layout (std140, binding = 2) uniform materialVal
{
    vec3 col;
} inMaterialVal;


layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inNor;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec3 outColor;

void main()
{
   outUV 		 = inUV;
   outColor		 = inMaterialVal.col;

   gl_Position 	 = vec4(inPos, 1) * inStaticVal.wvp;
}
