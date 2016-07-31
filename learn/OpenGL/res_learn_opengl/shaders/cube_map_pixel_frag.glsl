#version 400

layout (location = 0) out vec4 FragColor;

uniform samplerCube CubeMapTex; // The cube map

uniform bool DrawSkyBox;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

uniform vec3 worldCameraPos;

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
		vec4 cubeMapColor = texture(CubeMapTex, normalize(reflectDir));
		FragColor = mix(MaterialColor, cubeMapColor, ReflectFactor);
	}
}