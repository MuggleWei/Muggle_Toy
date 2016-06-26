#version 400

subroutine vec3 shadeModelType();
subroutine uniform shadeModelType shadeModel;

in vec3 Position;
in vec3 Normal;

struct PositionLightInfo
{
	vec4 Position;	// light position in eye space
	vec3 Intensity;	// light intensity
};
uniform PositionLightInfo PositionLight;

struct DirectionLightInfo
{
	vec4 Direction;	// light direction in eye space
	vec3 Intensity;	// light intensity
};
uniform DirectionLightInfo DirectionLight;

struct SpotLightInfo
{
	vec4 Position;
	vec3 Intensity;
	vec3 Direction;
	float Exponent;	// Angular attenuation exponent
	float Cutoff; 	// Cutoff angle (between 0 and 90)
};
uniform SpotLightInfo SpotLight;

struct MaterialInfo
{
	vec3 Ka;		// ambient reflectivity
	vec3 Kd;		// diffuse reflectivity
	vec3 Ks;		// specular reflectivity
	float Shininess;// specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

subroutine( shadeModelType )
vec3 PositionLightBlinnPhongModel()
{
	vec3 n = normalize(Normal);											// normal
	vec3 s = normalize(vec3(PositionLight.Position.xyz - Position));	// light dir
	vec3 v = normalize(-Position.xyz);									// view dir
	vec3 h = normalize(v + s);											// halfway vector
	
	float SDotN = max(dot(s, n), 0.0);
	float HDotN = max(dot(h, n), 0.0);
	
	return PositionLight.Intensity * (
		Material.Ka + 
		Material.Kd * SDotN + 
		Material.Ks * pow(HDotN, Material.Shininess)
	);
}

subroutine( shadeModelType )
vec3 DirectionLightBlinnPhongModel()
{
	vec3 n = normalize(Normal);											// normal
	vec3 s = normalize(vec3(-DirectionLight.Direction));				// light dir
	vec3 v = normalize(-Position.xyz);									// view dir
	vec3 h = normalize(v + s);											// halfway vector
	
	float SDotN = max(dot(s, n), 0.0);
	float HDotN = max(dot(h, n), 0.0);
	
	return DirectionLight.Intensity * (
		Material.Ka + 
		Material.Kd * SDotN + 
		Material.Ks * pow(HDotN, Material.Shininess)
	);
}

subroutine( shadeModelType )
vec3 SpotLightBlinnPhongModel()
{
	vec3 n = normalize(Normal);
	vec3 s = normalize( vec3( SpotLight.Position) - Position );
	
	float angle = acos( dot(-s, SpotLight.Direction) );
	float cutoff = radians( clamp( SpotLight.Cutoff, 0.0, 90.0 ) );
	
	vec3 ambient = SpotLight.Intensity * Material.Ka;
	if( angle < cutoff )
	{
		float spotFactor = pow( dot(-s, SpotLight.Direction), SpotLight.Exponent );
		
		vec3 v = normalize(vec3(-Position));
		vec3 h = normalize( v + s );
		
		float SDotN = max(dot(s, n), 0.0);
		float HDotN = max(dot(h, n), 0.0);
	
	return ambient + spotFactor * SpotLight.Intensity * (
			Material.Kd * SDotN +
			Material.Ks * pow(HDotN, Material.Shininess)
		);			
	}
	else
	{
		return ambient;
	}
}

void main()
{
	FragColor = vec4(shadeModel(), 1.0);
}