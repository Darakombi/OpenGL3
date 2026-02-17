#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TextureCoord;

out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	textureCoord = a_TextureCoord;

	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 textureCoord;

uniform sampler2D u_TextureColor;

void main()
{
	color = texture(u_TextureColor, textureCoord);
}