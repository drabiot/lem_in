#version 330 core

out vec4	FragColor;
  
in vec3	Color;
in vec2	Uv;

uniform sampler2D	tex;

void	main()
{
    FragColor = texture(tex, Uv);
}