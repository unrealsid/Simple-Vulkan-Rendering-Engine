//we will be using glsl version 4.5 syntax
#version 450

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

	//output the position of each vertex
	gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
}