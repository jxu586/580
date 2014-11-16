#version 400

in vec4 pos;
in vec3 n;
in vec2 uv;

uniform vec4 Color;

out vec4 ocol;
out vec4 opos;
out vec4 onormal;

void main()
{
	vec3 lightdir = vec3(1.0f, 2.0f, 3.0f);
	lightdir = normalize(lightdir);

	float k = clamp(dot(lightdir, n), 0.2f, 1.0f);

	ocol = Color * k;
	opos = pos;
	onormal.xyz = n;
}