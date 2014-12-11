#version 330 core

in vec3 EyeDirection_tangentspace;
in vec3 LightDirection_tangentspace;

in vec2 UV;
out vec3 color;

uniform vec3 lightColor;
uniform vec3 specularr;
uniform vec3 ambient;

uniform float power;


uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSamplerNormal;


void main()
{
	vec3 textureColor = texture(myTextureSampler, UV).rgb;
	vec3 ambientRez = ambient * textureColor;
	vec3 n = normalize(texture(myTextureSamplerNormal, UV).rgb * 2.0 - 1.0);
	vec3 l = normalize(LightDirection_tangentspace);
	vec3 diffuse = clamp(dot(n, l), 0, 1) * textureColor * lightColor * power;
	vec3 eye = normalize(EyeDirection_tangentspace);
	vec3 R = reflect(-l, n);
	float spec = clamp((dot(eye, R)), 0, 1);
	vec3 specular = specularr * pow(spec, 5);
	color = ambientRez + diffuse + specular;
}