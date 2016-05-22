#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo
{
	vec4 Position;	// light position in eye space
	vec3 La;		// ambient light intensity
	vec3 Ld;		// diffuse light intensity
	vec3 Ls;		// specular light intensity
};
uniform LightInfo Light;

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

vec3 PhongModel(vec4 position, vec3 norm)
{
	vec3 s = normalize(vec3(Light.Position - position));	// light dir
	vec3 v = normalize(-position.xyz);						// view dir
	vec3 r = reflect(-s, norm);								// reflect dir
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse
	float SDotN = max(dot(s, norm), 0.0);
	vec3 diffuse = Light.Ld * Material.Kd * SDotN;
	
	// specular
	vec3 spec = vec3(0.0);
	if (SDotN > 0)
	{
		spec = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
	}
	
	return ambient + diffuse + spec;
}

void main()
{
	vec3 eyeNormal;
	vec4 eyePosition;
	
	// get position and normal in eye space
	getEyeSpace(eyePosition, eyeNormal);
	
	// phong illumination model
	LightIntensity = PhongModel(eyePosition, eyeNormal);
	
	// position
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}