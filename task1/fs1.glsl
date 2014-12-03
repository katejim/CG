#version 330

in vec4 norm;
uniform bool wireframe;
out vec3 o_color;

void main()
{
    if(wireframe) 
        o_color = vec3(0, 0.015625, 1);    
    else 
        o_color = vec3(norm.y);    
}