#version 330 core

in vec3 in_pos;
in vec2 vertexUV;
in vec3 in_norm;
in vec3 tangents;
in vec3 bi_tangents;


out vec2 UV;
out vec3 EyeDirection_tangentspace;
out vec3 LightDirection_tangentspace;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;
uniform mat3 modelView3;
uniform vec3 lightPos;
uniform float q;

void main()
{
	gl_Position = mvp * vec4(in_pos, 1);
	UV = q > 0 ? vertexUV / q : vertexUV;
	vec3 vertexTangent_cameraspace = modelView3 * normalize(tangents);
	vec3 vertexNormal_cameraspace = modelView3 * normalize(in_norm);
	vec3 vertexBitangent_cameraspace = modelView3 * normalize(bi_tangents);
	mat3 TBN = transpose(mat3(
	                         vertexTangent_cameraspace,
	                         vertexBitangent_cameraspace,
	                         vertexNormal_cameraspace));
	LightDirection_tangentspace = TBN * (-lightPos);
	EyeDirection_tangentspace = TBN * (-(view * model * vec4(in_pos, 1)) .xyz);
}