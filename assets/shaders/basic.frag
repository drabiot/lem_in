#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
in vec3	pos;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
    //FragColor = texture(tex, TexCoord);
	FragColor = vec4(color, 1);
}