#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

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
uniform vec3 viewPos;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 normalDir = normalize(Normal);
	vec3 diffuse = max(dot(normalDir, lightDir), 0) * light.diffuse * material.diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalDir);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * light.specular * material.specular;

	float distance = length(light.position);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	FragColor = vec4(ambient + attenuation * (diffuse + specular), 1.0f);
}