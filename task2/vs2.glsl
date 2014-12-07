#version 330

in vec3 in_pos;
out vec3 position;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(in_pos, 1);
    position = in_pos;	
}