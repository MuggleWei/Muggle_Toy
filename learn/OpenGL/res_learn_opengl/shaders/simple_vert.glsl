#version 400
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;
out vec3 Color;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	Color = VertexColor;
	mat4 mvp_mat;
	mvp_mat = ProjectionMatrix * ViewMatrix * WorldMatrix;
	gl_Position = mvp_mat * vec4(VertexPosition, 1.0);
}