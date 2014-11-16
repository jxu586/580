#version 400

in vec4 pos;
in vec3 n;
in vec2 uv;

uniform vec3 EyeWorldPos;

uniform vec3 LightWorldPos;
uniform vec3 LightWorldDir;
uniform vec4 LightColor;
uniform float LightFOV;
uniform float LightAttenu;

uniform vec4 MatKa;
uniform vec4 MatKd;
uniform vec4 MatKs;
uniform float SpecExp;

uniform sampler2D TexMap;
uniform sampler2D ShadowMap;

out vec4 directColor;
out vec4 worldPos;
out vec4 worldNormal;

float ComputeShadowFactor()
{
	return 1.0f;
}

vec4 ComputeSpotLightModel(vec3 normal, vec3 pos, vec4 matColor)
{
	vec3 ldir = LightWorldPos - pos;
	float distSq = dot(ldir, ldir);
	float dist  = sqrt(distSq);
	float attenu = 1.0f / (0.8f + LightAttenu * dist + LightAttenu * dist * dist);
	ldir /= dist;

	float cosAngle = (1.0f - dot(ldir, LightWorldDir)) / (1.0f - LightFOV);
	float angleAttenu = clamp(pow(1.0f - cosAngle, 0.4f), 0.2f, 1.0f);


	//float cosAngle = dot(ldir, LightWorldDir);
	//if (cosAngle < LightFOV) {
	//	return matColor * 0.2f;
	//}	
	
	vec3 edir = normalize(EyeWorldPos - pos);
	vec3 edirRefl = 2 * normal * dot(normal, edir) - edir;
	
	float kd = clamp(dot(ldir, normal), 0.2f, 1.0f);
	float ks = clamp(dot(ldir, edirRefl), 0.0f, 1.0f);
	
	return (matColor * kd + MatKs * pow(ks, SpecExp)) * LightColor * attenu * angleAttenu;
}

void main()
{
	float v = dot(MatKd.xyz, MatKd.xyz);
	vec4 matColor = v < 0.00001f ? texture(TexMap, uv) : MatKd;
	vec4 finalColor = ComputeSpotLightModel(n, pos.xyz, matColor);
	
	directColor = finalColor;
	worldPos = pos;
	worldNormal.xyz = n;
}