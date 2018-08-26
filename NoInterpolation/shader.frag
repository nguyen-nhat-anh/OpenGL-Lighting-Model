#version 330 core

flat in vec3 VertexPositions[3]; // Triangle's vertices positions
flat in vec3 VertexColors[3]; // Triagle's vertices colors
in vec3 FragmentPosition;

out vec4 FragColor;

void main()
{
	vec3 barycentricCoord;
	barycentricCoord[0] = length(cross(FragmentPosition - VertexPositions[1], FragmentPosition - VertexPositions[2])) /
						  length(cross(VertexPositions[0] - VertexPositions[1], VertexPositions[0] - VertexPositions[2]));
	barycentricCoord[1] = length(cross(FragmentPosition - VertexPositions[0], FragmentPosition - VertexPositions[2])) /
						  length(cross(VertexPositions[1] - VertexPositions[0], VertexPositions[1] - VertexPositions[2]));
	barycentricCoord[2] = 1 - barycentricCoord[0] - barycentricCoord[1];
	FragColor = vec4(barycentricCoord[0] * VertexColors[0] + barycentricCoord[1] * VertexColors[1] + barycentricCoord[2] * VertexColors[2], 1.0f);
}