
#version 330

in vec2 in_pos;
out vec2 pos_surface;
uniform mat4 mvp;
uniform mat4 modelview;

void main()
{
   vec4 pos = vec4(in_pos, 0, 1);
   gl_Position = mvp * vec4(in_pos, 0, 1);
   pos_surface = (modelview * pos) .xy;     
}
