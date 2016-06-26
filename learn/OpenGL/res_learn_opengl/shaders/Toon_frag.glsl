#version 400

in vec3 Position;
in vec3 Normal;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

struct PositionLightInfo
{
	vec4 Position;	// light position in eye space
	vec3 Intensity;	// light intensity
};
uniform PositionLightInfo PositionLight;

struct MaterialInfo
{
	vec3 Ka;		// ambient reflectivity
	vec3 Kd;		// diffuse reflectivity
	vec3 Ks;		// specular reflectivity
	float Shininess;// specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

vec3 toonShadeModel()
{
	vec3 n = normalize(Normal);											// normal
	vec3 s = normalize(vec3(PositionLight.Position.xyz - Position));	// light dir
	float SDotN = max(dot(s, n), 0.0);
	
	vec3 diffuse = Material.Kd * floor( SDotN * levels ) * scaleFactor;
	return PositionLight.Intensity * (Material.Ka + diffuse);
}

void main()
{
	FragColor = vec4(toonShadeModel(), 1.0);
}