/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 21:44:20 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/13 17:57:59 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gui.h>
#include <openGLMagic.h>

/* 
typedef struct	s_guiElement
{
	GLuint		VAO;
	GLuint		VBO;
	vec2		pos;
	vec2		size;
	mat4		model;
	t_texture	*texture;
	char		*text;
	int			*itext;
	vec3		color;
	bool		isClickable;
	void		*action;
}				t_guiElement;
 */


t_texture	*font = NULL;
shaderID	fontShader = 0;

void	initGui()
{
	font = loadTexture(DefaultFontPath);
	if (!font)
		printf(RED"font texture failed for gui init\n"BASE_COLOR);
	fontShader = createShader(DefaultFontShaderVertPath, DefaultFontShaderFragPath);
	if (!fontShader)
		printf(RED"font shader failed for gui init\n"BASE_COLOR);
}

void	terminateGui()
{
	if (!font)
		return ;
	deleteTexture(font);
	if (fontShader)
		deleteShader(fontShader);
}

t_guiElement	*createGuiElement()
{
	t_guiElement	*elem;

	elem = malloc(sizeof(*elem));
	if (!elem)
		return (NULL);
	elem->VAO = 0;
	elem->VBO = 0;
	glGenVertexArrays(1, &elem->VAO);
	glGenBuffers(1, &elem->VBO);
	glBindVertexArray(elem->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, elem->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultRect), DefaultRect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glm_vec2_zero(elem->pos);
	glm_vec2_zero(elem->size);
	elem->scale = 1;
	elem->texture = NULL;
	elem->text = NULL;
	elem->itext = malloc(sizeof(*elem->itext) * 128);
	glm_vec3_one(elem->color);
	elem->itextLen = 0;
	elem->isClickable = false;
	elem->action = NULL;
	return (elem);
}

void	deleteGuiElement(t_guiElement *elem)
{
	if (!elem)
		return ;
	if (elem->VAO)
		glDeleteVertexArrays(1, &elem->VAO);
	elem->VAO = 0;
	if (elem->VBO)
		glDeleteVertexArrays(1, &elem->VBO);
	elem->VBO = 0;
	if (elem->text)
		free(elem->text);
	if (elem->itext)
		free(elem->itext);
	free(elem);
}

void	drawGui(t_guiElement *elem)
{
	if (!elem)
		return ;
	bindShader(fontShader);
	glDisable(GL_DEPTH_TEST);
	setFloat(fontShader, "scale", elem->scale);
	setVec2(fontShader, "size", elem->size);
	setVec2(fontShader, "pos", elem->pos);
	setVec2(fontShader, "aspect", (vec2){window->width, window->height});
	setVec2(fontShader, "fontSize", DefaultFontSize);
	setarray(fontShader, "text", elem->itext, elem->itextLen);
	setVec3(fontShader, "color", elem->color);
	useTexture(elem->texture, fontShader, "tex", 1);
	useTexture(font, fontShader, "font", 2);
	glBindVertexArray(elem->VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(DefaultRect));
	glEnable(GL_DEPTH_TEST);
	bindShader(0);
}

void	setGuiPos(t_guiElement *elem, vec2 npos)
{
	if (!elem)
		return ;
	glm_vec2_add(GLM_VEC2_ZERO, npos, elem->pos);
}

void	setGuiSize(t_guiElement *elem, vec2 nsize)
{
	if (!elem)
		return ;
	glm_vec2_add(GLM_VEC2_ZERO, nsize, elem->size);
}

void	setGuiScale(t_guiElement *elem, float nscale)
{
	if (!elem)
		return ;
	elem->scale = nscale;
}

void	setGuiText(t_guiElement *elem, char *ntext)
{
	int	i;
	
	if (!elem || (ntext && ft_strlen(ntext) > 128))
		return ;
	if (elem->text)
		free(elem->text);
	elem->text = ntext;
	elem->itextLen = ft_strlen(elem->text);
	i = 0;
	while (i < 128)
	{
		if (i < elem->itextLen)
			elem->itext[i] = (int)elem->text[i];
		else
			elem->itext[i] = 0;
		i ++;
	}
}

void	setGuiTextf(t_guiElement *elem, char *format, ...)
{
	int		i;
	char	*ntext;
	va_list	var;
	
	if (!elem || !format || (format && ft_strlen(format) > 128))
		return ;
	ntext = ft_calloc(sizeof(char), 129);
	va_start(var, format);
	vsnprintf(ntext, 128, format, var);
	if (ft_strlen(ntext) < 128 && elem->text)
		free(elem->text);
	if (ft_strlen(ntext) < 128)
	{
		elem->text = ntext;
		elem->itextLen = ft_strlen(elem->text);
		i = 0;
		while (i < 128)
		{
			if (i < elem->itextLen)
				elem->itext[i] = (int)elem->text[i];
			else
				elem->itext[i] = 0;
			i ++;
		}
	}
}

void	fitGuiToText(t_guiElement *elem, float ratio)
{
	float	textSize;

	if (ratio > 1 || ratio < 0)
		ratio = 1;
	textSize = ft_strlen(elem->text);
	glm_vec2_add(GLM_VEC2_ZERO, (vec3){ceil(textSize * ratio) * 10, floor(textSize - (textSize * (1 - ratio))) * 10, 0}, elem->size);
	while (elem->size[0] / 10 * elem->size[1] / 10 < textSize)
		elem->size[0] += 10;
	if (ratio == 1)
		elem->size[1] = 10;
}
