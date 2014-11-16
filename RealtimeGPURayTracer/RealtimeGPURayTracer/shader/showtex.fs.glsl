#version 400

in vec2 t;

//layout (pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D Tex;

out vec4 col;

void main()
{
//  vec2 tt;
//  tt.x= gl_FragCoord.x/1279;
//  tt.y =gl_FragCoord.y/719;

//  vec4 color = texture2D(Tex, tt);

	vec4 color = texture2D(Tex, t);
	col = color;
}