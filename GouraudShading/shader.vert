#version 330 core
layout (location = 0) in vec3 aPos; // vertex position (local coordinates)
layout (location = 1) in vec3 aNormal; // vertex normal vector (local coordinates)

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos; // eye's/camera's position in world coordinates

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0f)); // convert vertex position in local coordinates to world coordinates
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal; // convert vertex position in local coordinates to world coordinates

	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 normalDir = normalize(Normal);
	vec3 diffuse = max(dot(normalDir, lightDir), 0) * light.diffuse * material.diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalDir);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * light.specular * material.specular;

	float distance = length(light.position);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	Color = ambient + attenuation * (diffuse + specular);

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}