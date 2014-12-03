#version 330

in vec2 in_pos;
out vec2 pos_surface;
uniform mat4 mvp;

void main()
{
   gl_Position = mvp * vec4(in_pos, 0, 1);
   pos_surface = in_pos;     
}
