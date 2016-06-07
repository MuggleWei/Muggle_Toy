// fragment shader, Phong Illumination models, Phong shading Interpolation
#version 400

subroutine vec3 shadeModelType();
subroutine uniform shadeModelType shadeModel;

in vec3 Position;
in vec3 Normal;
in vec3 WorldNormal;

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

subroutine( shadeModelType )
vec3 AmbientColor()
{
	return Light.La * Material.Ka;
}

subroutine( shadeModelType )
vec3 DiffuseColor()
{
	vec3 n = normalize(Normal);
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
	vec3 s = normalize(vec3(Light.Position.xyz - Position));
	float SDotN = max(dot(s, n), 0.0);
	return Light.Ld * Material.Kd * SDotN;
}

subroutine( shadeModelType )
vec3 PhongSpecularColor()
{
	vec3 n = Normal;
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
	vec3 s = normalize(vec3(Light.Position.xyz - Position));
	vec3 v = normalize(-Position.xyz);
	vec3 r = reflect(-s, n);
	float RDotV = max(dot(r, v), 0.0);
	return Light.Ls * Material.Ks * pow(RDotV, Material.Shininess);
}

subroutine( shadeModelType )
vec3 BlinnPhongSpecularColor()
{
	vec3 n = normalize(Normal);	
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
	vec3 s = normalize(vec3(Light.Position.xyz - Position));
	vec3 v = normalize(-Position.xyz);
	vec3 h = normalize(v + s);
	float HDotN = max(dot(h, n), 0.0);
	return Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
}

subroutine( shadeModelType )
vec3 PhongModel()
{
	vec3 n = normalize(Normal);									// normal
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
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

subroutine( shadeModelType )
vec3 BlinnPhongModel()
{
	vec3 n = normalize(Normal);									// normal
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
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

subroutine(shadeModelType)
vec3 NormalColor()
{
	vec3 n = normalize(WorldNormal);								// normal
	if( !gl_FrontFacing ) 
	{
		n = -n;
	}
	return n;
}

void main()
{
	FragColor = vec4(shadeModel(), 1.0);
}