#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 in_pos;
in vec2 vertexUV;
in vec3 in_norm;
in vec3 tangents;
in vec3 bi_tangents;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;
uniform mat3 modelView3;
uniform vec3 lightPos;
uniform float q;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvp * vec4(in_pos,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (model * vec4(in_pos,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( view * model* vec4(in_pos,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( view * vec4(lightPos,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// UV of the vertex. No special space for this one.
	UV = q > 0 ? vertexUV/q : vertexUV;
	
	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = modelView3 * tangents;
	vec3 vertexBitangent_cameraspace = modelView3 * bi_tangents;
	vec3 vertexNormal_cameraspace = modelView3 * in_norm;
	
	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace	
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
	
	
}

