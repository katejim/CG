#version 330

in vec2 pos_surface;
out vec3 o_color;
const float checkSize = 0.15;

void main()
{
	vec3 color;
	vec2 position = pos_surface / checkSize;
	if (((fract(position.x) < 0.5) && (fract(position.y) < 0.5)) || ((fract(position.x) > 0.5) && (fract(position.y) > 0.5)))    
        o_color = vec3(0, 0.015625, 1);    
    else    
        o_color = vec3(1, 1, 1);
}