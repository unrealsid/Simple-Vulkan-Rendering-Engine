#version 450

//Output
layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 outUVTexCoord;

//Inputs
layout (location = 0 ) in vec3 vPosition;
layout (location = 1 ) in vec3 vNormal;
layout (location = 2 ) in vec3 vColor;
layout (location = 3 ) in vec2 vUvTexCoord;

void main()
{
	//output the position of each vertex
	gl_Position = vec4(vPosition, 1.0f);
	outColor = vColor;
	outUVTexCoord = vUvTexCoord;
}