// fragment shader, Phong Illumination models, Phong shading Interpolation
#version 400

in vec3 Position;
in vec3 Normal;

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

layout( location = 0 ) out vec4 FragColor;

vec3 PhongModel()
{
	vec3 n = normalize(Normal);									// normal
	vec3 s = normalize(vec3(Light.Position.xyz - Position));	// light dir
	vec3 v = normalize(-Position.xyz);							// view dir
	vec3 r = reflect(-s, Normal);								// reflect dir
	
	float SDotN = max(dot(s, n), 0.0);
	float RDotV = max(dot(r, v), 0.0);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse
	
	vec3 diffuse = Light.Ld * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(RDotV, Material.Shininess);
	
	return ambient + diffuse + spec;
}

void main()
{
	FragColor = vec4(PhongModel(), 1.0);
}