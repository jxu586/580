#version 400

layout (location = 0) in vec3 Position;

out vec2 t;

void main()
{
	gl_Position = vec4(Position, 1.0f);
	t = Position.xy * 0.5f + vec2(0.5f, 0.5f);
}
