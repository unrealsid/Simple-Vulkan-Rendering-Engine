//glsl version 4.5
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;

//output write
layout (location = 0) out vec4 outoutFragColor;

layout(set = 0, binding = 1) uniform GlobalData
{   
	vec4 time;
	vec4 textureIdx;
	vec4 resolution;
} globalData;

layout(set = 2, binding = 0) uniform sampler samplerObj;
layout(set = 2, binding = 1) uniform texture2D textures[2];

#define MAX_STEPS 128
#define STEP_SIZE 0.05
#define _FoV 45.0

#define JITTER

#define ROTATE

const vec3 _Absorption = vec3(0.9,0.7,0.5);
const vec3 _Emission = vec3(0.3,0.15,0.0);
const vec3 _Light = vec3(1.0,1.0,1.0);
const vec3 _LightPos = vec3(0.0, 0.0, 0.0);
const float _LightIntensity = 0.2;
const float _Density = 9.0;
const float _Radius = 1.0;
const float _Mie = 0.6;
const float _G = 0.2;

//Cornette-Shanks phase function
float phase(float mu, float g)
{
	float g2 = g * g;
	return (3.0 * (1.0 - g2) * (1.0 + mu * mu)) / (2.0 * (2.0 + g2) * pow(1.0 + g2 - 2.0 * g * mu, 1.5));
}

//Ray-sphere intersection
bool raycastSphere(vec3 ro, vec3 rd, out vec3 p0, out vec3 p1, vec3 center, float r)
{
    float A = 1.0; //dot(rd, rd);
    float B = 2.0 * (rd.x * (ro.x - center.x) + rd.y * (ro.y - center.y) + rd.z * (ro.z - center.z));
    float C = dot(ro - center, ro - center) - (r * r);

    float D = B * B - 4.0 * A * C;
    if (D < 0.0)
    {
        return false;
    }
    else
    {
        float t0 = (-B - D)/(2.0 * A);
        float t1 = (-B + D)/(2.0 * A);
        p0 = ro + rd * t0;
        p1 = ro + rd * t1;
        return true;
    }
}

//Y-axis rotation matrix
vec3 rotateY(vec3 p, float t)
{
    float cosTheta = cos(t);
    float sinTheta = sin(t);
    mat3 rot = mat3(cosTheta, 0.0, sinTheta,
        			0.0, 1.0, 0.0,
    			    -sinTheta, 0.0, cosTheta);
    
    return rot * p;
}

//iq's gradient noise
vec3 hash( vec3 p )
{
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec3 p )
{
    vec3 i = floor( p );
    vec3 f = fract( p );
	
	vec3 u = f*f*(3.0-2.0*f);

    return mix( mix( mix( dot( hash( i + vec3(0.0,0.0,0.0) ), f - vec3(0.0,0.0,0.0) ), 
                          dot( hash( i + vec3(1.0,0.0,0.0) ), f - vec3(1.0,0.0,0.0) ), u.x),
                     mix( dot( hash( i + vec3(0.0,1.0,0.0) ), f - vec3(0.0,1.0,0.0) ), 
                          dot( hash( i + vec3(1.0,1.0,0.0) ), f - vec3(1.0,1.0,0.0) ), u.x), u.y),
                mix( mix( dot( hash( i + vec3(0.0,0.0,1.0) ), f - vec3(0.0,0.0,1.0) ), 
                          dot( hash( i + vec3(1.0,0.0,1.0) ), f - vec3(1.0,0.0,1.0) ), u.x),
                     mix( dot( hash( i + vec3(0.0,1.0,1.0) ), f - vec3(0.0,1.0,1.0) ), 
                          dot( hash( i + vec3(1.0,1.0,1.0) ), f - vec3(1.0,1.0,1.0) ), u.x), u.y), u.z );
}

//Otavio Good's fast spiral noise from https://www.shadertoy.com/view/ld2SzK
const float nudge = 0.739513;	// size of perpendicular vector
float normalizer = 1.0 / sqrt(1.0 + nudge*nudge);	// pythagorean theorem on that perpendicular to maintain scale
float SpiralNoiseC(vec3 p)
{
    float n = 0.0;	// noise amount
    float iter = 1.0;
    for (int i = 0; i < 6; i++)
    {
        // add sin and cos scaled inverse with the frequency
        n += -abs(sin(p.y*iter) + cos(p.x*iter)) / iter;	// abs for a ridged look
        // rotate by adding perpendicular and scaling down
        p.xy += vec2(p.y, -p.x) * nudge;
        p.xy *= normalizer;
        // rotate on other axis
        p.xz += vec2(p.z, -p.x) * nudge;
        p.xz *= normalizer;
        // increase the frequency
        iter *= 1.733733;
    }
    return n;
}

//Fractal (pink) noise function S(f) ∝ 1/f^α, {0 <= α <=2}
float fbm(vec3 seed, int octaves, float freq, float lac)
{
    float val;
    float j = 1.0;
    for (int i = 0; i < octaves; i++, j+=1.0)
    {
        val += noise(seed * freq * j) / pow(j, lac);
    }

    return val;
}

//Density distribution function
float sampleVolume(vec3 pos)
{
    float rr = dot(pos,pos);
    rr = sqrt(rr);
    float f = exp(-rr);
    float p = f * _Density;
    
    if (p <= 0.0)
        return p;
    
    p += SpiralNoiseC(512.0 + pos * 8.0) * 0.75;
    pos = rotateY(pos, pos.y * SpiralNoiseC(pos * 4.0)* 2.0);
    p += SpiralNoiseC(200.0 + pos * 3.0) * 1.5;
    p *= rr/_Radius;
        
    p = max(0.0,p);
                
    return p;
}

vec4 raymarch(vec3 pos, vec3 dir, float ds, int s)
{
    vec4 result = vec4(0.,0.0,0.0,1.0);
    int steps = min(s, MAX_STEPS);
    for (int i = 0; i < steps; i++)
    {
        float p = sampleVolume(pos);
        if (p > 0.0)
        {
            vec3 r = _LightPos - pos;
            float atten = _LightIntensity/dot(r, r);
            vec3 ext = max(vec3(0.000001), (_Absorption * p) + vec3(_Mie * p));
            vec3 trans = exp(-ext * ds);
            vec3 lightDir = normalize(r);
            float mu = dot(lightDir, dir);
            float phase = phase(mu, _G);
            vec3 lum = _Emission + _Light * phase * (1.0-_Absorption) * _Mie * p * atten;
            vec3 integral = (lum - (lum*trans))/ext;
            
            result.rgb += integral * result.a;
            vec3 div = vec3(0.3333333);
            result.a *= dot(trans, div);
            
            if (result.a <= 0.1)
                return result;
        }
            
        pos += dir * ds;
    }
        
    return result;
}

//4x4 Bayer matrix for ordered dithering
const mat4 _Bayer4x4 = mat4(vec4(0,0.5,0.125,0.625),
                        vec4(0.75,0.25,0.875,.375), 
                        vec4(0.1875,0.6875,0.0625,0.5625), 
                        vec4(0.9375,0.4375,0.8125,0.3125));

void mainImage( out vec4 outFragColor, in vec2 gl_FragCoord )
{
    float zoom = 1.0f/globalData.resolution.y;
    vec3 rayOrigin = vec3(0.0, 0.0, -5.0 + (5.0 * zoom));

    //Compute eye vector from field of view
    vec2 uv = gl_FragCoord/globalData.resolution.xy;
    uv.y *= -1;

    float ar = globalData.resolution.x/globalData.resolution.y;
    float d = ar/tan(radians(_FoV/2.0));    
    vec3 rayDir = normalize(vec3((-1.0 + 2.0 * uv) * vec2(ar, 1.0), d));
    
    
//    #ifdef ROTATE
//	float t = iMouse.x * 0.01 + globalData.time.x * 0.1;
//    #else
//    float t = iMouse.x * 0.01;
//    #endif
    
    float t = globalData.time.x * 0.1;

    rayDir = rotateY(rayDir, t);
    rayOrigin = rotateY(rayOrigin, t);
    
    vec4 col = vec4(0.0,0.0,0.0,1.0);
    //col.rgb += smoothstep(0.45, 0.9, abs(noise(rayDir * 256.0)));
    float star = smoothstep(0.45, 0.8, abs(noise(rayDir * 256.0)));
    col.rgb += star * mix(vec3(1.0,0.7,0.2),vec3(0.0,0.5,1.0), star);
    
    vec3 p0, p1;
    if (raycastSphere(rayOrigin, rayDir, p0, p1, vec3(0.0), _Radius))
    {   
        p0 = (dot(rayOrigin, rayOrigin) < dot(p0, p0)) ? rayOrigin : p0;
        #ifdef JITTER
        //Bayer matrix ordered depth jittering
        float width = (uv.x * globalData.resolution.x);
        float height = (uv.y * globalData.resolution.y);
        width = mod(width, 4.0);
        height = mod(height, 4.0);
        float offset = _Bayer4x4[int(width)][int(height)];
        p0 -= rayDir * offset * STEP_SIZE*2.0;
        #endif
        
        float dist = length(p1 - p0);
        int s = int(dist/STEP_SIZE) + 1;
        
        vec4 integral = raymarch(p0, rayDir, STEP_SIZE, s);
                
        col.rgb = mix(integral.rgb, col.rgb, integral.a);
    }	
       
    outFragColor = col;
}