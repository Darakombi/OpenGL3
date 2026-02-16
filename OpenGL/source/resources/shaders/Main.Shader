#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	normal = mat3(transpose(inverse(model))) * a_Normal;
	fragPos = vec3(model * vec4(a_Pos, 1.0));
	
	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

in vec3 normal;
in vec3 fragPos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_LightPos - fragPos);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = diff * u_LightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = spec * u_LightColor;

	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	color = vec4(result, 1.0);
}