#version 330 core

int	charSize = 10; // a char in the font is 10 pixels

layout (location = 0) in vec2	aPos;
layout (location = 1) in vec2	aUv;

uniform vec2	size;
uniform vec2	pos;
uniform float	scale;
uniform vec2	fontSize;
uniform vec2	aspect;

out vec2		UV;
flat out vec2	realSize;

void	main ()
{
	gl_Position = vec4((aPos * (size / 2.) + (size / 4.)) / aspect * scale + pos, 0., 1.);
	UV = aUv * (size / charSize);
	realSize = (size / charSize);
}
