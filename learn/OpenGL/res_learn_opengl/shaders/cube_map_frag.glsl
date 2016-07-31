#version 400

layout (location = 0) out vec4 FragColor;

in vec3 ReflectDir;

uniform samplerCube CubeMapTex; // The cube map

uniform bool DrawSkyBox;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

void main()
{
	vec4 cubeMapColor = texture(CubeMapTex, normalize(ReflectDir));
	
	if (DrawSkyBox)
	{
		FragColor = cubeMapColor;
	}
	else
	{
		FragColor = mix(MaterialColor, cubeMapColor, ReflectFactor);
	}
}