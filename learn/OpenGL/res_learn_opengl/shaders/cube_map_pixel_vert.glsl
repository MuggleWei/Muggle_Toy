#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

uniform bool DrawSkyBox;
uniform vec3 worldCameraPos;

uniform mat4 ModelMatrix;
uniform mat4 MVP;

out vec3 worldPos;
out vec3 worldNorm;

void main()
{
	if (DrawSkyBox)
	{
		worldPos = VertexPosition;
	}
	else
	{
		worldPos = vec3(ModelMatrix * vec4(VertexPosition,1.0) );
		worldNorm = vec3(ModelMatrix * vec4(VertexNormal, 1.0));
	}
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}