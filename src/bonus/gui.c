/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 21:44:20 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/20 17:54:35 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gui.h>
#include <openGLMagic.h>

t_texture		*font = NULL;
shaderID		fontShader = 0;
shaderID		btnShader = 0;
t_guiElement	**buttons = NULL;;
int				buttonID = 0;
GLuint			buttonFrameBuffer = 0;
GLuint			buttonSelectTexture = 0;
GLuint			buttonRenderBuffer = 0;

bool	initGui()
{
	font = loadTexture(DefaultFontPath);
	if (!font)
		return (false);
	fontShader = createShader(DefaultFontShaderVertPath, DefaultFontShaderFragPath);
	if (!fontShader)
		return (false);
	btnShader = createShader(DefaultFontShaderVertPath, DefaultSelectShaderFragPath);
	if (!btnShader)
		return (false);
	glGenFramebuffers(1, &buttonFrameBuffer);
	glGenTextures(1, &buttonSelectTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, buttonFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, buttonSelectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ButtonBufferWidth, ButtonBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buttonSelectTexture, 0);
	glGenRenderbuffers(1, &buttonRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buttonRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ButtonBufferWidth, ButtonBufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buttonRenderBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return (false);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return (true);
}

void	terminateGui()
{
	if (!font)
		return ;
	deleteTexture(font);
	if (fontShader)
		deleteShader(fontShader);
	if (btnShader)
		deleteShader(btnShader);
	if (buttons)
		free(buttons);
	if (buttonFrameBuffer)
		glDeleteFramebuffers(1, &buttonFrameBuffer);
	if (buttonRenderBuffer)
		glDeleteRenderbuffers(1, &buttonRenderBuffer);
	if (buttonSelectTexture)
		glDeleteTextures(1, &buttonSelectTexture);
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
	elem->isButton = false;
	elem->isClickable = false;
	elem->buttonID = -1;
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

void	setGuiText(t_guiElement *elem, char *ntext, bool dup)
{
	int	i;
	
	if (!elem || (ntext && ft_strlen(ntext) > 128))
		return ;
	if (elem->text)
		free(elem->text);
	if (dup)
		elem->text = ft_strdup(ntext);
	else
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

void	setGuiColor(t_guiElement *elem, vec3 color)
{
	glm_vec3_add(GLM_VEC3_ZERO, color, elem->color);
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

static void	incrButtonsList(t_guiElement *nButton)
{
	t_guiElement	**nList;
	int				i;

	i = 0;
	while (buttons && buttons[i])
		i ++;
	nList = ft_calloc(sizeof(*nList), (i + 2));
	i = 0;
	while (buttons && buttons[i])
	{
		nList[i] = buttons[i];
		i ++;
	}
	nList[i] = nButton;
	nList[i + 1] = NULL;
	free(buttons);
	buttons = nList;
}

void	makeButton(t_guiElement *elem, void *func)
{
	if (!elem)
		return ;

	elem->isButton = true;
	elem->isClickable = true;
	elem->action = func;
	elem->buttonID = buttonID ++;
	incrButtonsList(elem);
}

void	doAction(t_guiElement *elem)
{
	elem->action(elem);
}

void	setButtonStatus(t_guiElement *elem, bool status)
{
	if (elem && elem->isButton)
		elem->isClickable = status;
}

void	checkButtons()
{
	int				i;
	unsigned char	data[3];
	
	bindShader(btnShader);
	
	glBindFramebuffer(GL_FRAMEBUFFER, buttonFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, buttonSelectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->width, window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glBindRenderbuffer(GL_RENDERBUFFER, buttonRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->width, window->height);

	glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	i = -1;
	while (buttons[++i])
	{
		if (!buttons[i]->isClickable)
			continue;

		glDisable(GL_DEPTH_TEST);
		setVec3(btnShader, "color", (vec3){(float)(buttons[i]->buttonID + 1) / 255.f, 0, 0});
		setFloat(btnShader, "scale", buttons[i]->scale);
		setVec2(btnShader, "size", buttons[i]->size);
		setVec2(btnShader, "pos", buttons[i]->pos);
		setVec2(btnShader, "aspect", (vec2){window->width, window->height});

		glBindVertexArray(buttons[i]->VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(DefaultRect));
	}

	glReadPixels(getMousePosX(), window->height - getMousePosY(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

	if (data[0] && data[0] <= buttonID)
		doAction(buttons[data[0] - 1]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bindShader(0);
	glEnable(GL_DEPTH_TEST);
}
