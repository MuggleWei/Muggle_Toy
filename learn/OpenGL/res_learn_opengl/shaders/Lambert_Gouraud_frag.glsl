// fragment shader, Lambert Illumination models, Gouraud shading Interpolation
#version 400

in vec3 LightIntensity;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(LightIntensity, 1.0);
}