/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 21:41:49 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/20 17:53:54 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUI_H
# define GUI_H

# include <texture.h>

# include <cglm/cglm.h>

# include <stdarg.h>

void	*ft_calloc(size_t size, size_t count);

# define DefaultFontPath				"assets/textures/defaultFont.bmp"
# define DefaultFontShaderVertPath		"assets/shaders/font.vert"
# define DefaultFontShaderFragPath		"assets/shaders/font.frag"
# define DefaultSelectShaderFragPath	"assets/shaders/selection.frag"
# define DefaultFontSize				(vec2){26, 2} // number of char spaces in the texture (size / 10)
# define ButtonBufferWidth				1200
# define ButtonBufferHeight				800

//									╒═══ POS ═══╕╒═══ UV ═══╕
# define DefaultRect	((float[30]){0.5f,  0.5f, 1.0f, 1.0f,\
									 0.5f, -0.5f, 1.0f, 0.0f,\
									-0.5f,  0.5f, 0.0f, 1.0f,\
									 0.5f, -0.5f, 1.0f, 0.0f,\
									-0.5f, -0.5f, 0.0f, 0.0f,\
									-0.5f,  0.5f, 0.0f, 1.0f})

typedef struct	s_guiElement
{
	GLuint		VAO;
	GLuint		VBO;
	vec2		pos;
	vec2		size;
	float		scale;
	t_texture	*texture;
	char		*text;
	int			*itext;
	vec3		color;
	int			itextLen;
	bool		isButton;
	bool		isClickable;
	int			buttonID;
	void		(*action)(struct s_guiElement *);
}				t_guiElement;

bool	initGui();
void	terminateGui();

t_guiElement	*createGuiElement();
void			deleteGuiElement(t_guiElement *elem);

void	drawGui(t_guiElement *elem);
void	setGuiPos(t_guiElement *elem, vec2 npos);
void	setGuiSize(t_guiElement *elem, vec2 nsize);
void	setGuiScale(t_guiElement *elem, float nscale);
void	setGuiText(t_guiElement *elem, char *ntext, bool dup);
void	setGuiTextf(t_guiElement *elem, char *format, ...);
void	setGuiColor(t_guiElement *elem, vec3 color);
void	fitGuiToText(t_guiElement *elem, float ratio);

void	makeButton(t_guiElement *elem, void *func);
void	doAction(t_guiElement *elem);
void	setButtonStatus(t_guiElement *elem, bool status);
void	checkButtons();

#endif