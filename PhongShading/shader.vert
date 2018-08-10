#version 330 core
layout (location = 0) in vec3 aPos; // vertex position (local coordinates)
layout (location = 1) in vec3 aNormal; // vertex normal vector (local coordinates)

out vec3 FragPos; // vertex position (world coordinates)
out vec3 Normal; // vertex normal vector (world coordinates)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0f)); // convert vertex position in local coordinates to world coordinates
	Normal = mat3(transpose(inverse(model))) * aNormal; // convert vertex position in local coordinates to world coordinates
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}