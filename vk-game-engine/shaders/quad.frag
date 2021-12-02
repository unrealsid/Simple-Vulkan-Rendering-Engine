//glsl version 4.5
#version 450

layout (location = 0) in vec3 inColor;

layout(set = 0, binding = 1) uniform GlobalData
{   
	vec4 lightDir;
	float time;
} globalData;

//output write
layout (location = 0) out vec4 outFragColor;

void main()
{
	//return red
	outFragColor = vec4(inColor + vec3(globalData.time), 1.0f);
}