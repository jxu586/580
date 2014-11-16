#version 400

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Texcoord;

uniform mat4 WMatrix;
uniform mat4 VPMatrix;

out vec4 pos;
out vec3 n;
out vec2 uv;

void main()
{
	// world position
	vec3 worldPos = (WMatrix * vec4(Position.xyz, 1.0f)).xyz;
	pos.xyz = worldPos;
	pos.w = gl_VertexID / 3 + 1;
	// final position
	gl_Position = VPMatrix * vec4(worldPos.xyz, 1.0f);	
	// world normal 
	n = (WMatrix * vec4(Normal.xyz, 0.0f)).xyz;
	uv = Texcoord.xy;
}