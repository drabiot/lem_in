#version 330 core

layout (location = 0) in vec3	aPos;
layout (location = 1) in vec3	aNormal;
layout (location = 2) in vec2	aUv;

uniform mat4	proj;
uniform mat4	view;
uniform mat4	model;

out vec3	Normal;
out vec2	Uv;

uniform sampler2D	tex;

void	main()
{
    Normal = normalize(abs(aNormal));
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    Uv = aUv;
}