#version 330 core

float	charSize = 10; // a char in the font is 10 pixels

// first font row
float	charSpace	= 32;	// -> ' '
float	charComa	= 44;	// -> ','
float	charMinus	= 45;	// -> ','
float	charDot		= 46;	// -> ','
float	char0		= 48;	// ┬> [0:9], ':', ';'
float	charSemicol	= 59;	// ┘

// second font row
float	charCapA	= 65;	// ┬> [A:Z] ┬> treated the same at the end
float	charCapZ	= 90;	// ┘        │
float	charA		= 97;	// ┬> [a:z] ┘
float	charZ		= 122;	// ┘

out vec4	FragColor;

flat in vec2	realSize;
in vec2			UV;

uniform vec2		fontSize;
uniform vec3		color;
uniform sampler2D	font;
uniform int[128]	text;

vec2	FUV;


vec2	getPos(float char)
{
	// return (vec2(0, 0));

	if (char == charSpace)
		return (vec2(0, 0));
	if (char == charComa || char == charDot)
		return (vec2(1. / fontSize.x, 0));
	if (char == charMinus)
		return (vec2(2. / fontSize.x, 0));

	if (char >= char0 && char <= charSemicol)
		return (vec2((char - char0 + 3.) / fontSize.x, 0));

	if (char >= charCapA && char <= charCapZ)
		return (vec2((char - charCapA) / fontSize.x, 1 / fontSize.y));
	if (char >= charA && char <= charZ)
		return (vec2((char - charA) / fontSize.x, 1 / fontSize.y));
	return (vec2(1, 0));
}

void main()
{
	FUV = fract(UV) / fontSize;
	int[3] ttest = int[3](int(charA), int(charA), int(charA));
	
	FragColor = texture(font, getPos(text[int(floor(UV.x) + floor(realSize.y - UV.y) * realSize.x)]) + FUV);
	FragColor = vec4(color * FragColor.rgb, FragColor.r);
}
