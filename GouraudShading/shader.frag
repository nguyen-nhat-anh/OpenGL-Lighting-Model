#version 330 core

in vec3 Color; // fragment color (interpolated)

out vec4 FragColor;

void main()
{
	FragColor = vec4(Color, 1.0f);
}