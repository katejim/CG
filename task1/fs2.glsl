
#version 330

in vec3 position;


uniform vec3 center;
uniform float size;
uniform float k;
uniform float v;
uniform float dist;
uniform float time;

out vec3 o_color;

void main()
{    
    o_color = vec3(sin(2*3.14* (v*time + length(position - center) * k / dist)));
}