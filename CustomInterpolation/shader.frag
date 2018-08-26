#version 330 core

in vec3 barycentricCoord;
flat in vec3 Colors[3];

out vec4 FragColor;

void main()
{
	FragColor = vec4(barycentricCoord[0] * Colors[0] + barycentricCoord[1] * Colors[1] + barycentricCoord[2] * Colors[2], 1.0f);
}