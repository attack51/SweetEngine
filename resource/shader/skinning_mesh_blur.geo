#version 450
precision highp int;
precision highp float;

///////////////// uniform buffer ///////////////////////

///////////////// ~uniform buffer ///////////////////////

layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

layout(location = 0) in vec4[] inExtendPos;
layout(location = 1) in vec2[] inVelocityNDC;

layout(location = 0) out vec2 outVelocityNDC;

void main() 
{
	vec4 original[3];
	vec4 extends[3];
	for(uint i=0; i<3; ++i)
	{
		original[i] = gl_in[i].gl_Position;
		extends[i] = inExtendPos[i];
	}

	////////////////////////////////////////
	//bottom
	for(uint i=0; i<3; ++i)
	{
		gl_Position = original[i];
		outVelocityNDC = inVelocityNDC[i];
		EmitVertex();
	}
    EndPrimitive();

	////////////////////////////////////////
	//top
	for(uint i=0; i<3; ++i)
	{
		gl_Position = extends[i];
		outVelocityNDC = inVelocityNDC[i];
		EmitVertex();
	}
    EndPrimitive();
	
	////////////////////////////////////////
	//extended edge
	for(uint i=0; i<4; ++i)
	{
		uint index = i%3;
		gl_Position = original[index];
		outVelocityNDC = inVelocityNDC[index];
		EmitVertex();

		gl_Position = extends[index];
		outVelocityNDC = inVelocityNDC[index];
		EmitVertex();
	}
	
    EndPrimitive();
}  