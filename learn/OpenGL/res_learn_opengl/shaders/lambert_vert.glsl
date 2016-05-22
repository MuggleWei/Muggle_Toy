#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPositionInEye;	// light position in eye space
uniform vec3 Kd;					// Diffuse reflectivity
uniform vec3 Ld;					// Light intensity

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
	// Convert normal and position to eye space
	vec3 norm = normalize(NormalMatrix * VertexNormal);
	vec4 posInEye = ModelViewMatrix * vec4(VertexPosition, 1.0);
	
	// get light direction in eye space
	vec3 s = normalize(vec3(LightPositionInEye - posInEye));
	
	LightIntensity = Ld * Kd * max(dot(s, norm), 0.0);
	
	// Convert position to ndc space
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}