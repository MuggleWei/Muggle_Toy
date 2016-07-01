// fragment shader, Blinn-Phong Illumination models, Phong shading Interpolation
#version 400

subroutine vec4 shadeModelType();
subroutine uniform shadeModelType shadeModel;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D DiffuseTex;
uniform sampler2D MossTex;

uniform float blendFactor;

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
vec4 BlendTexture()
{
	vec3 n = normalize(Normal);									// normal
	vec3 s = normalize(vec3(Light.Position.xyz - Position));	// light dir
	vec3 v = normalize(-Position.xyz);							// view dir
	vec3 h = normalize(v + s);									// halfway vector
	
	float SDotN = max(dot(s, n), 0.0);
	float HDotN = max(dot(h, n), 0.0);
	
	// diffuse texture color
	vec4 diffuseColor = texture( DiffuseTex, TexCoord );
	vec4 mossColor = texture(MossTex, TexCoord);
	vec3 texColor = mix(diffuseColor.xyz, mossColor.xyz, mossColor.a);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse	
	vec3 diffuse = Light.Ld * texColor.xyz * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
	
	return vec4(ambient + diffuse + spec, 1.0);
}

subroutine( shadeModelType )
vec4 AlphaDiscard()
{
	// diffuse texture color
	vec4 diffuseColor = texture( DiffuseTex, TexCoord );
	vec4 mossColor = texture(MossTex, TexCoord);
	vec3 texColor;
	
	texColor = mix(diffuseColor.xyz, mossColor.xyz, mossColor.a);
	mossColor.a = mossColor.a * blendFactor;
	if (mossColor.a > 0.8)
	{
		discard;
	}
	
	vec3 n = normalize(Normal);									// normal
	vec3 s = normalize(vec3(Light.Position.xyz - Position));	// light dir
	vec3 v = normalize(-Position.xyz);							// view dir
	vec3 h = normalize(v + s);									// halfway vector
	
	float SDotN = max(dot(s, n), 0.0);
	float HDotN = max(dot(h, n), 0.0);
	
	// ambient
	vec3 ambient = Light.La * Material.Ka;
	
	// diffuse	
	vec3 diffuse = Light.Ld * texColor.xyz * Material.Kd * SDotN;
	
	// specular
	vec3 spec = Light.Ls * Material.Ks * pow(HDotN, Material.Shininess);
	
	return vec4(ambient + diffuse + spec, 1.0);
}

void main()
{
	FragColor = shadeModel();
}