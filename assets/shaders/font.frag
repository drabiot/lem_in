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


vec2	getPos(float Char)
{
	if (Char == charSpace)
		return (vec2(0, 0));
	if (Char == charComa || Char == charDot)
		return (vec2(1. / fontSize.x, 0));
	if (Char == charMinus)
		return (vec2(2. / fontSize.x, 0));

	if (Char >= char0 && Char <= charSemicol)
		return (vec2((Char - char0 + 3.) / fontSize.x, 0));

	if (Char >= charCapA && Char <= charCapZ)
		return (vec2((Char - charCapA) / fontSize.x, 1 / fontSize.y));
	if (Char >= charA && Char <= charZ)
		return (vec2((Char - charA) / fontSize.x, 1 / fontSize.y));
	return (vec2(1, 0));
}

void main()
{
	FUV = fract(UV) / fontSize;
	int[3] ttest = int[3](int(charA), int(charA), int(charA));
	
	FragColor = texture(font, getPos(text[int(floor(UV.x) + floor(realSize.y - UV.y) * realSize.x)]) + FUV);
	FragColor = vec4(color * FragColor.rgb, FragColor.r);
}
