#version 330 core

in vec3 FragPos; // fragment position (interpolated)
flat in vec3 VertexPos[3]; // triangle's vertices position (world coordinates)
flat in vec3 VertexNormal[3]; // triangle's vertices normal vector (world coordinates)

out vec4 FragColor; // fragment color

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
	// Linear interpolation explicit formula
	// -------------------------------------
	vec3 barycentricCoord;
	barycentricCoord[0] = length(cross(FragPos - VertexPos[1], FragPos - VertexPos[2])) /
						  length(cross(VertexPos[0] - VertexPos[1], VertexPos[0] - VertexPos[2]));
	barycentricCoord[1] = length(cross(FragPos - VertexPos[0], FragPos - VertexPos[2])) /
						  length(cross(VertexPos[1] - VertexPos[0], VertexPos[1] - VertexPos[2]));
	barycentricCoord[2] = 1 - barycentricCoord[0] - barycentricCoord[1];
	
	// Fragment's normal vector in world coordinates
	vec3 Normal = barycentricCoord[0] * VertexNormal[0] + barycentricCoord[1] * VertexNormal[1] + barycentricCoord[2] * VertexNormal[2];
	// -------------------------------------
	
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 normalDir = normalize(Normal);
	vec3 diffuse = max(dot(normalDir, lightDir), 0) * light.diffuse * material.diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalDir);
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * light.specular * material.specular;

	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	FragColor = vec4(ambient + attenuation * (diffuse + specular), 1.0f);
}