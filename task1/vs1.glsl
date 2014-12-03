#version 330

in vec3 in_pos;
in vec3 in_norm;
out vec4 norm;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(in_pos, 1);	
    norm = model * vec4(in_norm, 1);
}