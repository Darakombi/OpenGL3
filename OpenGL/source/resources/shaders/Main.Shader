#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	texCoord = a_TexCoord;
	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex_diffuse1;

void main()
{
	color = texture(tex_diffuse1, texCoord), 1.0;
}