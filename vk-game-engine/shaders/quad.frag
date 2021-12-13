//glsl version 4.5
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;

//output write
layout (location = 0) out vec4 outFragColor;

layout(set = 0, binding = 1) uniform GlobalData
{   
	vec4 time;
	vec4 textureIdx;
} globalData;

layout(set = 2, binding = 0) uniform sampler samplerObj;
layout(set = 2, binding = 1) uniform texture2D textures[2];

void main()
{
	vec3 color = texture(sampler2D(textures[int(globalData.textureIdx.x)], samplerObj), inUV).xyz + globalData.time.x;
	outFragColor = vec4(color, 1.0f);
}