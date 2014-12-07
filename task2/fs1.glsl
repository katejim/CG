#version 330

in vec4 norm;
uniform bool wireframe;


in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
    color = texture( myTextureSampler, UV ).rgb;  
}