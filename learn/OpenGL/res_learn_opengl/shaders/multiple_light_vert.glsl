#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Color;

struct LightInfo
{
	vec4 Position;	// light position in eye space
	vec3 Intensity;	// light intensity
};
uniform LightInfo lights[5];

struct MaterialInfo
{
	vec3 Ka;		// ambient reflectivity
	vec3 Kd;		// diffuse reflectivity
	vec3 Ks;		// specular reflectivity
	float Shininess;// specular shininess factor
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void getEyeSpace(out vec4 position, out vec3 norm)
{
	position = ModelViewMatrix * vec4(VertexPosition, 1.0);
	norm = normalize(NormalMatrix * VertexNormal);
}

vec3 PhongModel(int light_idx, vec4 position, vec3 norm)
{
	vec3 s = normalize(vec3(lights[light_idx].Position - position));	// light dir
	vec3 v = normalize(-position.xyz);						// view dir
	vec3 r = reflect(-s, norm);								// reflect dir
	
	float SDotN = max(dot(s, norm), 0.0);
	float RDotV = max(dot(r, v), 0.0);
	
	return lights[light_idx].Intensity * (Material.Ka
		+ Material.Kd * SDotN
		+ Material.Ks * pow(RDotV, Material.Shininess));
}

void main()
{
	vec3 eyeNormal;
	vec4 eyePosition;
	
	// get position and normal in eye space
	getEyeSpace(eyePosition, eyeNormal);
	
	// phong illumination model
	Color = vec3(0.0);
	for (int i = 0; i < 5; ++i)
	{
		Color += PhongModel(i, eyePosition, eyeNormal);
	}
	
	// position
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}