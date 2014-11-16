#version 400

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Texcoord;

uniform mat4 WMatrix;
uniform mat4 WVPMatrix;

out vec4 pos;
out vec3 n;
out vec2 uv;

void main()
{
	// final position
	gl_Position = WVPMatrix * vec4(Position.xyz, 1.0f);
	// world position
	pos.xyz = (WMatrix * vec4(Position.xyz, 1.0f)).xyz;
	pos.w = gl_VertexID / 3 + 1;
	
	// world normal 
	n = (WMatrix * vec4(Normal.xyz, 0.0f)).xyz;
	uv = Texcoord.xy;
}