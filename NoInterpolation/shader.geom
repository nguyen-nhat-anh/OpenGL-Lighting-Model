#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 Position[];
in vec3 Color[];

flat out vec3 VertexPositions[3];
flat out vec3 VertexColors[3];
out vec3 FragmentPosition;

void main()
{
	// these variables are not getting interpolated
	for (int i = 0; i < 3; i++)
	{
		VertexColors[i] = Color[i];
		VertexPositions[i] = Position[i];
	}

	// these variables will be interpolated for each fragment
	for (int i = 0; i < 3; i++)
	{
		FragmentPosition = Position[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}