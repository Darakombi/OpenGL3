#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texPos;

out vec2 tex_coords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(pos, 1.0);
	tex_coords = texPos;
};

#shader fragment
#version 330 core

in vec2 tex_coords;

uniform sampler2D tex;
uniform sampler2D tex2;

out vec4 color;

void main()
{
	color = mix(texture(tex, tex_coords), texture(tex2, tex_coords), 0.3);
};