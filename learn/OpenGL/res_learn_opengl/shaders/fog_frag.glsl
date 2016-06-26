#version 400

subroutine float FogFactor(float dist);
subroutine uniform FogFactor fogFactor;

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

struct FogInfo
{
	float maxDist;
	float minDist;
	float density;
	vec3 color;
};
uniform FogInfo Fog;

layout( location = 0 ) out vec4 FragColor;

vec3 BlinnPhongModel()
{
	vec3 n = normalize(Normal);									// normal
	vec3 s = normalize(vec3(Light.Position.xyz - Position));	// light dir
	vec3 v = normalize(-Position.xyz);							// view dir
	vec3 h = normalize(v + s);									// halfway vector
	
	float SDotN = max(dot(s, n), 0.0);
	float HDotN = max(dot(h, n), 0.0);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse
	
	vec3 diffuse = Light.Ld * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
	
	return ambient + diffuse + spec;
}

subroutine( FogFactor )
float linearFog(float dist)
{
	float fogFactor = (Fog.maxDist - dist) / (Fog.maxDist - Fog.minDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	return fogFactor;
}

subroutine( FogFactor )
float exponentialFog(float dist)
{
	float fogFactor = exp(-Fog.density * dist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	return fogFactor;
}

void main()
{
	// float dist = abs(Position.z);
	float dist = length( Position.xyz );
	float fogFactor = fogFactor(dist);
	
	vec3 originColor = BlinnPhongModel();
	vec3 color = mix(Fog.color, originColor, fogFactor);

	FragColor = vec4(color, 1.0);
}