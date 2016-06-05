#version 400

subroutine vec3 shadeModelType(vec4 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 FrontColor;
out vec3 BackColor;

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
uniform mat3 WorldNormalMatrix;

void getEyeSpace(out vec4 position, out vec3 norm)
{
	position = ModelViewMatrix * vec4(VertexPosition, 1.0);
	norm = normalize(NormalMatrix * VertexNormal);
}

subroutine( shadeModelType )
vec3 PhongModel(vec4 position, vec3 norm)
{
	vec3 s = normalize(vec3(Light.Position - position));	// light dir
	vec3 v = normalize(-position.xyz);						// view dir
	vec3 r = reflect(-s, norm);								// reflect dir
	r = normalize(r);
	
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

subroutine( shadeModelType )
vec3 lambertModel( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(Light.Position - position));
	return Light.Ld * Material.Kd * max(dot(s, norm), 0.0);
}

subroutine( shadeModelType )
vec3 NormalColor(vec4 position, vec3 norm)
{
	vec3 color = normalize(WorldNormalMatrix * VertexNormal);
	color = (color + 1) / 2.0;
	return color;
}

subroutine( shadeModelType )
vec3 AbsoluteNormalColor(vec4 position, vec3 norm)
{
	vec3 color = normalize(WorldNormalMatrix * VertexNormal);	
	return color;
}


void main()
{
	vec3 eyeNormal;
	vec4 eyePosition;
	
	// get position and normal in eye space
	getEyeSpace(eyePosition, eyeNormal);
	
	// phong illumination model
	FrontColor = shadeModel(eyePosition, eyeNormal);
	BackColor = shadeModel(eyePosition, -eyeNormal);
	
	// position
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}