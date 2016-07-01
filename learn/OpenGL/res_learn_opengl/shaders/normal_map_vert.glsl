// vertex shader, Blinn-Phong Illumination models, Phong shading Interpolation
#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec3 VertexBitangent;

out vec2 TexCoord;
out vec3 LightDir;
out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;		// normal matrix for object normal to 
uniform mat4 MVP;

struct LightInfo
{
	vec4 Position;	// light position in eye space
	vec3 La;		// ambient light intensity
	vec3 Ld;		// diffuse light intensity
	vec3 Ls;		// specular light intensity
};
uniform LightInfo Light;

void main()
{
	// note: this is right hand coordinate
	vec3 Normal = normalize(NormalMatrix * VertexNormal);
	vec3 Tangent = normalize(NormalMatrix * VertexTangent);
	vec3 Bitangent = normalize(NormalMatrix * VertexBitangent);
	
	// matrix for transform from eye space to tangent space 
	// note1: glsl matrix is column major order
	mat3 toObjectLocal = mat3(
		Tangent.x, Bitangent.x, Normal.x, 
		Tangent.y, Bitangent.y, Normal.y, 
		Tangent.z, Bitangent.z, Normal.z
	);
	
	// position in eye
	vec3 Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
	
	// transform light dir and view dir to tangent space
	LightDir = normalize(toObjectLocal * (Light.Position.xyz - Position));
	ViewDir = toObjectLocal * normalize(-Position);
	
	TexCoord = VertexTexCoord;
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}