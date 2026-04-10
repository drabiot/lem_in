#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
in vec3	pos;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
	vec3 npos = abs(normalize(pos));
    FragColor = texture(tex, TexCoord);
	FragColor = vec4(vec3(npos.x, npos.y, npos.z), 1);
}