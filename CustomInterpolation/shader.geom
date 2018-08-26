#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 Color[];
flat out vec3 Colors[3];
out vec3 barycentricCoord;

void main()
{
	for (int i = 0; i < 3; i++)
	{
		Colors[i] = Color[i];
	}
	for (int i = 0; i < 3; i++)
	{
		barycentricCoord = vec3(0.0f);
		barycentricCoord[i] = 1.0f;

		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}