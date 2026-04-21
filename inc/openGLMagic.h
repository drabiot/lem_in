/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   openGLMagic.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 17:17:57 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/20 18:21:16 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGLMAGIC_H
# define OPENGLMAGIC_H

# include <color.h>
# include <camera.h>
# include <shader.h>

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>

# include <unistd.h>

# define DefaultWindowidth 900
# define DefaultWindoheight 900
# define DefaultWindoname "lem_in"
# define Max_Keys 512
# define Max_Mouse_Keys 12

typedef struct	s_windowInfo
{
	GLFWwindow	*windowData;
	int			width;
	int			height;
	char		*name;
	double		mousePos[2];
	vec2		mouseDelta;
	bool		keysSates[Max_Keys];
	bool		mouseSates[Max_Mouse_Keys];
	vec2		mouseScroll;
	bool		isRuning;
	bool		lockMouse;
	float		pastTime;
	float		deltaTime;
	t_camera	*camera;
	bool		is2Dcam;
}				t_windowInfo;

extern t_windowInfo	*window;

t_windowInfo	*launchOpenGL(void);

void	startFrame();
void	endFrame();

void	refreshMouse();
float	getMousePosX();
float	getMousePosY();

float	getWindowW();
float	getWindowH();

bool	isKeyPressed(const int key);
bool	isKeyRepeated(const int key);
bool	isMousePressed(const int key);
bool	isMouseRepeated(const int key);
float	getMouseScrollX();
float	getMouseScrollY();

#endif