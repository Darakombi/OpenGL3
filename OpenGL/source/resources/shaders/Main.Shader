#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoord;

out vec3 normal;
out vec3 fragPos;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	normal = mat3(transpose(inverse(model))) * a_Normal;
	fragPos = vec3(model * vec4(a_Pos, 1.0));
	textureCoord = a_TextureCoord;

	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

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
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp(0.0, (theta - light.outerCutoff) / epsilon, 1.0);

	vec3 ambient = light.ambient * texture(material.diffuse, textureCoord).rgb;

	vec3 norm = normalize(normal);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoord).rgb;
	
	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, textureCoord).rgb;

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	//ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 lm = ambient + diffuse + specular;

	color = vec4(lm, 1.0);
}