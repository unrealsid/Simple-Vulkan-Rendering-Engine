//we will be using glsl version 4.5 syntax
#version 450

layout (location = 0) out vec3 outColor;

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
	gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
	outColor = colors[gl_VertexIndex];
}