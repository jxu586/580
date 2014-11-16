#version 400

in vec4 pos;
in vec3 n;
in vec2 uv;

uniform vec3 LightWorldPos;
uniform vec3 LightWorldDir;
uniform vec4 LightColor;
uniform float LightFOV;
uniform float LightAttenu;

uniform vec4 MatKd;
uniform vec4 ReflectiveCoeff;

uniform sampler2D TexMap;

out vec4 flux;
out vec4 worldPos;
out vec4 worldNormal;

void main()
{
	float v = dot(MatKd.xyz, MatKd.xyz);
	vec4 matColor = v < 0.00001f ? texture(TexMap, uv) : MatKd;
	
	vec3 ldir = LightWorldPos - pos.xyz;
	float distSq = dot(ldir, ldir);
	float dist = sqrt(distSq);
	
	ldir /= dist;
	float cosAngle = (1.0f - dot(ldir, LightWorldDir)) / (1.0f - LightFOV);
	float angleAttenu = pow(1.0f - cosAngle, 0.4f);
	float distAttenu = 1.0f / (0.8f + LightAttenu * dist + LightAttenu * dist * dist);
	
	flux = LightColor * matColor * ReflectiveCoeff * angleAttenu * distAttenu;
	flux.w = 1.0f;
	worldPos = pos;
	worldNormal.xyz = n;
}