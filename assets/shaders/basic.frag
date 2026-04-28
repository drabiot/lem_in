#version 330 core

out vec4	FragColor;
  
in vec3		Normal;
in vec2		Uv;
in float	offset;

uniform sampler2D	tex;

vec3	col1 = vec3(0.27450980392156865, 0.1450980392156863, 0.0392156862745098);
vec3	col2 = vec3(0.3333333333333333, 0.1843137254901961, 0.07058823529411765) * 1.5;

void	main()
{
	vec4	tcolor = vec4(0.);
	float	total = 0;
	for (int y = -5; y <= 5; ++y)
	{
   		for (int x = -5; x <= 5; ++x)
		{
			tcolor += texture(tex, Uv + (0.0005 * x, 0.0005 * y));
			total ++;
		}
	}
	tcolor /= total;
	
	
	FragColor = (vec4(1., 1., 1., 2.) - tcolor) * vec4(mix(col2, col1, offset), 1.);
	// FragColor = vec4(Normal, 1.);
	FragColor += vec4(mix(col1, col2, offset), 1.);
}