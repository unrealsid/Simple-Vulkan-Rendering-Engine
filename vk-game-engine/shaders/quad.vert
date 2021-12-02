#version 450

//Output
layout (location = 0) out vec3 outColor;

//Inputs
layout (location = 0 ) in vec3 vPosition;
layout (location = 1 ) in vec3 vNormal;
layout (location = 2 ) in vec3 vColor;
layout (location = 3 ) in vec2 uv;

layout(set = 0, binding = 0) uniform  CameraBuffer
{
	mat4 view;
	mat4 proj;
	mat4 viewProjection;
	vec4 translate;
} cameraData;


void main()
{
	//const array of positions for the quad
	const vec3 positions[6] = vec3[6]
	(
		//Tri 1
		vec3( -0.5f,  -0.5f, 0.0f),
		vec3(0.5f, -0.5f, 0.0f),
		vec3(0.5f, 0.5f, 0.0f), 

		//Tri 2
		vec3(0.5f, 0.5f, 0.0f), 
		vec3( -0.5f,  0.5f, 0.0f),
		vec3( -0.5f,  -0.5f, 0.0f)
	);

	//const array of colors for the triangle
	const vec3 colors[6] = vec3[6]
	(
		vec3(1.0f, 0.0f, 0.0f), //red
		vec3(0.0f, 1.0f, 0.0f), //green
		vec3(00.f, 0.0f, 1.0f),  //blue

		vec3(1.0f, 0.0f, 0.0f), //red
		vec3(0.0f, 1.0f, 0.0f), //green
		vec3(00.f, 0.0f, 1.0f)  //blue
	);

	//output the position of each vertex
	gl_Position = vec4(vPosition + vec3(cameraData.translate), 1.0f);
	outColor = vColor;
}