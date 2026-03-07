#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	fragPos = vec3(model * vec4(a_Pos, 1.0));
	normal = mat3(transpose(inverse(model))) * a_Normal;
	texCoord = a_TexCoord;

	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material{
	sampler2D diffuse1;
	sampler2D specular1;
	float shininess1;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 u_ViewPos;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse1, texCoord).rgb;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse1, texCoord).rgb;

	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
	vec3 specular = light.specular * spec * texture(material.specular1, texCoord).rgb;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0);
}