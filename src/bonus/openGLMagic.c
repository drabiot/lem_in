/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   openGLMagic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 17:17:36 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/22 17:35:56 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <openGLMagic.h>

t_windowInfo	*window = NULL;

static bool	openWindow(t_windowInfo *window);
static void	setupWindow(t_windowInfo *window);

t_windowInfo	*launchOpenGL(void)
{
	int	i;

	i = 0;
	window = malloc(sizeof(*window));
	window->windowData = NULL;
	window->width = DefaultWindowidth;
	window->height = DefaultWindoheight;
	window->name = DefaultWindoname;
	window->mousePos[0] = 0;
	window->mousePos[1] = 0;
	while (i < Max_Keys)
	{
		if (i < Max_Mouse_Keys)
			window->mouseSates[i] = false;
		window->keysSates[i] = false;
		i ++;
	}
	window->isRuning = true;
	window->lockMouse = false;
	window->pastTime = 0;
	window->deltaTime = 0;
	window->camera = createCam(GLM_VEC3_ZERO, 90, 0);
	openWindow(window);
	window->camera->aspectRatio = window->width / window->height;
	updateCam(window->camera, true);
	window->is2Dcam = false;
	return (window);
}

void	startFrame()
{
	glfwPollEvents();

	glClearColor(0.4f, 0.6f, 0.8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window->pastTime = glfwGetTime();
}

void	refreshMouse()
{
	glfwGetCursorPos(window->windowData, &window->mousePos[0], &window->mousePos[1]);
	if (window->lockMouse)
		glfwSetCursorPos(window->windowData, window->width / 2.f, window->height / 2.f);
}

void	endFrame()
{
	static vec2	pastMousePos;
	vec2		mousePos;

	refreshMouse();
	glfwSwapBuffers(window->windowData);
	
	glm_vec2_zero(window->mouseScroll);

	mousePos[0] = window->mousePos[0];
	mousePos[1] = window->mousePos[1];
	if (window->is2Dcam)
		glm_vec2_sub(mousePos, pastMousePos, window->mouseDelta);
	else
		glm_vec2_zero(window->mouseDelta);
	glm_vec2_add(GLM_VEC2_ZERO, mousePos, pastMousePos);

	window->deltaTime = window->pastTime - glfwGetTime();

	if (glfwWindowShouldClose(window->windowData))
		window->isRuning = false;
}

float	getMousePosX()
{
	return (window->mousePos[0]);
}

float	getMousePosY()
{
	return (window->mousePos[1]);
}

float	getWindowW()
{
	return (window->width);
}

float	getWindowH()
{
	return (window->height);
}

bool	isKeyPressed(const int key)
{
	bool	keyState;

	if (key >= Max_Keys || key < 0)
		return (false);
	keyState = window->keysSates[key];
	window->keysSates[key] = false;
	return (keyState);
}

bool	isKeyRepeated(const int key)
{
	if (key >= Max_Keys || key < 0)
		return (false);
	return (window->keysSates[key]);
}

bool	isMousePressed(const int key)
{
	bool	mouseState;

	if (key >= Max_Mouse_Keys || key < 0)
		return (false);
	mouseState = window->mouseSates[key];
	window->mouseSates[key] = false;
	return (mouseState);
}

bool	isMouseRepeated(const int key)
{
	if (key >= Max_Mouse_Keys || key < 0)
		return (false);
	return (window->mouseSates[key]);
}

float	getMouseScrollX()
{
	return (window->mouseScroll[0]);
}

float	getMouseScrollY()
{
	return (window->mouseScroll[1]);
}

static bool	openWindow(t_windowInfo *window)
{
	GLFWmonitor	*monitor = NULL;

	if (!glfwInit())
		return (false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	window->windowData = glfwCreateWindow(window->width, window->height,
		window->name, monitor, NULL);
	if (!window->windowData)
	{
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(window->windowData);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return (false);
	}
	setupWindow(window);
	return (true);
}

static void	opengErrorMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	char	*severityColor;
	
	severityColor = WHITE;
	(void)source, (void)type, (void)id, (void)severity, (void)length, (void)message, (void)userParam;
	if (severity == GL_DEBUG_SEVERITY_LOW)
			severityColor = GREEN;
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
			severityColor =  YELLOW;
	else if (severity == GL_DEBUG_SEVERITY_HIGH)
			severityColor =  RED;
	write(2, "Opengl Message: ", 17);
	write(2, severityColor, ft_strlen(severityColor));
	if (type == GL_DEBUG_TYPE_ERROR)
		write(2, "ERROR; ", 8);
	else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
		write(2, "DEPRECATED_BEHAVIOR; ", 22);
	else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		write(2, "UNDEFINED_BEHAVIOR; ", 21);
	else if (type == GL_DEBUG_TYPE_PORTABILITY)
		write(2, "PORTABILITY; ", 14);
	else if (type == GL_DEBUG_TYPE_PERFORMANCE)
		write(2, "PERFORMANCE; ", 14);
	else if (type == GL_DEBUG_TYPE_OTHER)
		write(2, "OTHER; ", 8);
	else
		write(2, "NO TYPE; ", 10);
	write(2, message, ft_strlen((char *)message));
	write(2, BASE_COLOR"\n", 1 + ft_strlen(BASE_COLOR));
}

static void	resizeCallBack(GLFWwindow *windowData, int width, int height)
{
	(void)windowData;
	glViewport(0, 0, width, height);
	window->camera->aspectRatio = (float)width / (float)height;
	window->width = width;
	window->height = height;
}

void	keyCallBack(GLFWwindow *windowData, int key, int scancode, int action, int mods)
{
	(void)windowData;
	(void)scancode;
	(void)mods;
	if (action == GLFW_PRESS || (action == GLFW_REPEAT && window->keysSates[key]))
		window->keysSates[key] = true;
	else
		window->keysSates[key] = false;
}

void	mouseCallBack(GLFWwindow *windowData, int button, int action, int mods)
{
	(void)windowData;
	(void)mods;
	if (action == GLFW_PRESS)
		window->mouseSates[button] = true;
	else
		window->mouseSates[button] = false;
}

void	mouseScrollCallBack(GLFWwindow *windowData, double xoffset, double yoffset)
{
	(void)windowData;
	window->mouseScroll[0] = xoffset;
	window->mouseScroll[1] = yoffset;
}

void	focusCallBack(GLFWwindow *windowData, int focused)
{
	(void)windowData;
	(void)focused;
	window->lockMouse = false;
	glfwSetInputMode(window->windowData, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

static void	setupWindow(t_windowInfo *window)
{
	glfwGetWindowSize(window->windowData, &window->width, &window->height);
	glViewport(0, 0, window->width, window->height);
    glDebugMessageCallback(opengErrorMsg, 0);
	glfwSetInputMode(window->windowData, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetFramebufferSizeCallback(window->windowData, resizeCallBack);
	glfwSetKeyCallback(window->windowData, keyCallBack);
	glfwSetMouseButtonCallback(window->windowData, mouseCallBack);
	glfwSetScrollCallback(window->windowData, mouseScrollCallBack);
	glfwSetWindowFocusCallback(window->windowData, focusCallBack);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetCursorPos(window->windowData, window->width / 2.f, window->height / 2.f);
	glfwGetCursorPos(window->windowData, &window->mousePos[0], &window->mousePos[1]);
	glClearColor(0.4f, 0.4f, 0.6f, 1.f);
}
