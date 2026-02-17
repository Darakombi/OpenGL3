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

in vec3 normal;
in vec3 fragPos;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

uniform sampler2D u_TextureColor;
in vec2 textureCoord;

void main()
{
	vec3 ambient = material.ambient * light.ambient;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = material.diffuse * light.diffuse * diff;
	
	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * light.specular * spec;
	
	vec3 result = (ambient + diffuse + specular) * u_LightColor;

	vec4 tex = texture(u_TextureColor, textureCoord);
	color = vec4(result, 1.0) * tex;
}