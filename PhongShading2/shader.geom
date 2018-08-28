#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 Normal[];
flat out vec3 VertexNormal[3];

in vec3 Pos[];
flat out vec3 VertexPos[3];

out vec3 FragPos;


void main()
{
	for (int i = 0; i < 3; i++)
	{
		VertexNormal[i] = Normal[i];
		VertexPos[i] = Pos[i];
	}
	for (int i = 0; i < 3; i++)
	{
		FragPos = Pos[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}