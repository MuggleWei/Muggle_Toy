// fragment shader, Blinn-Phong Illumination models, Phong shading Interpolation
#version 400

subroutine vec4 shadeModelType();
subroutine uniform shadeModelType shadeModel;

in vec2 TexCoord;
in vec3 LightDir;
in vec3 ViewDir;

uniform sampler2D DiffuseTex;
uniform sampler2D NormalTex;

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
vec4 diffuseModel()
{
	vec4 diffuseTexColor = texture( DiffuseTex, TexCoord );
	vec4 n = vec4(0.0, 0.0, 1.0, 1.0);

	vec3 s = normalize(LightDir);			// light dir
	vec3 v = normalize(ViewDir);			// view dir
	vec3 h = normalize(v + s);				// halfway vector
	
	float SDotN = max(dot(s, n.xyz), 0.0);
	float HDotN = max(dot(h, n.xyz), 0.0);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse	
	vec3 diffuse = Light.Ld * diffuseTexColor.xyz * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
	
	return vec4(ambient + diffuse + spec, 1.0);
}

subroutine( shadeModelType )
vec4 diffuseNormalModel()
{
	vec4 diffuseTexColor = texture( DiffuseTex, TexCoord );
	vec4 n = texture( NormalTex, TexCoord );
	n.rgb = normalize(n.rgb * 2.0 - vec3(1.0));

	vec3 s = normalize(LightDir);			// light dir
	vec3 v = normalize(ViewDir);			// view dir
	vec3 h = normalize(v + s);				// halfway vector
	
	float SDotN = max(dot(s, n.xyz), 0.0);
	float HDotN = max(dot(h, n.xyz), 0.0);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse	
	vec3 diffuse = Light.Ld * diffuseTexColor.xyz * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
	
	return vec4(ambient + diffuse + spec, 1.0);	
}

void main()
{
	FragColor = shadeModel();
}