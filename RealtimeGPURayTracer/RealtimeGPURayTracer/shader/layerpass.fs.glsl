#version 400

in vec2 t;

layout (pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D Min;
uniform sampler2D Max;

uniform int Width;
uniform int Height;

out vec4 minB;
out vec4 maxB;

void main()
{
    float w = (Width - 1);
	float h = (Height - 1);

	float sw = (Width/2 -1);
	float sh = (Height/2 -1);
	
	vec2 f1 ;
	vec2 f2;
	vec2 f3;
	vec2 f4;

    f1.x =  (t.x * 2 * sw)/w ;
	f1.y = (t.y * 2 * sh)/h ;
	f2.x =  (t.x * 2 * sw +1)/w;
	f2.y =  (t.y * 2 * sh)/h ;
    f3.x =  (t.x * 2 * sw)/w;
	f3.y = (t.y * 2 * sh + 1)/h;
	f4.x =  (t.x * 2 * sw +1)/w ;
	f4.y = (t.y * 2 * sh + 1)/h ; 
	  
/*  vec2 f1;
	vec2 f2;
	vec2 f3;
	vec2 f4;
	f1.x = (gl_FragCoord.x *2)/w;
	f1.y = (gl_FragCoord.y * 2)/h;
    f2.x = (gl_FragCoord.x * 2 +1)/w;
	f2.y = (gl_FragCoord.y * 2 )/h;
	f3.x =  (gl_FragCoord.x * 2 )/w ;
	f3.y =  (gl_FragCoord.y * 2 + 1)/h;
	f4.x =  (gl_FragCoord.x * 2  +1)/w;
	f4.y =  (gl_FragCoord.y * 2  + 1)/h; */

//	vec4 cc= texture(Min,f1);

	vec4 min1 = texture(Min, f1);
	vec4 min2 = texture(Min, f2);
	vec4 min3 = texture(Min, f3);
	vec4 min4 = texture(Min, f4);

	vec4 max1 = texture(Max, f1);
	vec4 max2 = texture(Max, f2);
	vec4 max3 = texture(Max, f3);
	vec4 max4 = texture(Max, f4);

	vec4 temp1;
	temp1.x = min(min(min1.x,min2.x) ,min(min3.x,min4.x));
	temp1.y = min(min(min1.y,min2.y) ,min(min3.y,min4.y));
	temp1.z = min(min(min1.z,min2.z) ,min(min3.z,min4.z));
	temp1.w = min(min(min1.w,min2.w) ,min(min3.w,min4.w));
	
	vec4 temp2;
	temp2.x =  max(max(max1.x,max2.x) ,max(max3.x,max4.x));
	temp2.y =  max(max(max1.y,max2.y) ,max(max3.y,max4.y));
	temp2.z =  max(max(max1.z,max2.z) ,max(max3.z,max4.z));
	temp2.w =  max(max(max1.w,max2.w) ,max(max3.w,max4.w));  

	minB = temp1;
	maxB = temp2;

}