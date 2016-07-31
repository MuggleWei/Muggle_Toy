#version 400

layout (location = 0) out vec4 FragColor;

uniform samplerCube CubeMapTex; // The cube map

uniform bool DrawSkyBox;
uniform vec4 MaterialColor;
uniform vec3 worldCameraPos;

struct MaterialInfo
{
	float Eta; 				// Ratio of indices of refraction
	float ReflectFactor;	// percentage of reflected light
};
uniform MaterialInfo materialInfo;

in vec3 worldPos;
in vec3 worldNorm;

void main()
{	
	if (DrawSkyBox)
	{
		vec4 cubeMapColor = texture(CubeMapTex, normalize(worldPos));
		FragColor = cubeMapColor;
	}
	else
	{
		vec3 norm = normalize(worldNorm);
		vec3 worldView = worldCameraPos - worldPos;
		worldView = normalize(worldView);
		
		vec3 reflectDir = reflect(-worldView, norm );
		vec3 refractDir = refract(-worldView, norm, materialInfo.Eta);
		
		vec4 reflectColor = texture(CubeMapTex, normalize(reflectDir));
		vec4 refractColor = texture(CubeMapTex, normalize(refractDir));
		
		FragColor = mix(refractColor, reflectColor, materialInfo.ReflectFactor);
	}
}