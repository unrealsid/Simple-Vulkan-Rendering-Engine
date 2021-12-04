//glsl version 4.5
#version 450

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;

layout(set = 0, binding = 0) uniform ShaderInputs
{   
	vec4 resolution;
	vec4 time;
} shaderInputs;

layout( set = 1, binding = 1) uniform sampler2D imgTexture;

//output write
layout (location = 0) out vec4 outFragColor;

float noise(vec3 p) //Thx to Las^Mercury
{
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}

float sphere(vec3 p, vec4 spr)
{
	return length(spr.xyz-p) - spr.w;
}

float flame(vec3 p)
{
	float d = sphere(p*vec3(1.,.5,1.), vec4(.0,-1.,.0,1.));
	return d + (noise(p+vec3(.0,shaderInputs.time.x*2.,.0)) + noise(p*3.)*.5)*.25*(p.y) ;
}

float scene(vec3 p)
{
	return min(100.-length(p) , abs(flame(p)));
}

vec4 raymarch(vec3 org, vec3 dir)
{
	float d = 0.0, glow = 0.0, eps = 0.02;
	vec3  p = org;
	bool glowed = false;
	
	for(int i=0; i<64; i++)
	{
		d = scene(p) + eps;
		p += d * dir;

		if( d>eps )
		{
			if(flame(p) < .0)
				glowed=true;
			if(glowed)
       			glow = float(i)/64.;
		}
	}
	return vec4(p,glow);
}

void main()
{
	vec2 v = -1.0 + 2.0 * gl_FragCoord.xy / shaderInputs.resolution.xy;
	v.x *= shaderInputs.resolution.x / shaderInputs.resolution.y;
	v.y *= -1.f;
	
	vec3 org = vec3(0., -2., 4.);
	vec3 dir = normalize(vec3(v.x*1.6, -v.y, -1.5));
	
	vec4 p = raymarch(org, dir);
	float glow = p.w;
	
	vec4 col = mix(vec4(1.,.5,.1,1.), vec4(0.1,.5,1.,1.), p.y * .02 + .4);
	
	outFragColor = mix(vec4(0.), col, pow(glow*2.,4.));
	//fragColor = mix(vec4(1.), mix(vec4(1.,.5,.1,1.),vec4(0.1,.5,1.,1.),p.y*.02+.4), pow(glow*2.,4.));
}


//void main()
//{
//	//outFragColor = vec4(inColor + vec3(shaderInputs.time), 1.0f);
//
//	vec3 color = vec3(shaderInputs.time);
//	outFragColor = vec4(color, 1.0f);
//}