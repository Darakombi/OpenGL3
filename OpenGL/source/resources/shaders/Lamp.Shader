#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

uniform vec3 u_Color;

void main()
{
	color = vec4(u_Color, 1.0);
}