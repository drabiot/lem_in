/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/08 17:02:05 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>

void	launchOpenGl(void);



void	freeFarm(t_AntFarm *farm)
{
	int	i;

	i = 0;
	while (farm->room[i])
	{
		free(farm->room[i]->ID);
		free(farm->room[i]->neighbours);
		free(farm->room[i]);
		i++;
	}
	free(farm->room);
	free(farm->nbAntstr);
}

void	freePaths(t_path **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]->path);
		free(paths[i]);
		i ++;
	}
	free(paths);
}

void	computeDistToEnd(t_room *room, int distToEnd)
{
	int	i;

	i = 0;
	room->distToEnd = distToEnd;
	while (room->neighbours[i])
	{
		if (room->neighbours[i]->distToEnd > distToEnd + 1)
			computeDistToEnd(room->neighbours[i], distToEnd + 1);
		i ++;
	}
}

int	main(void)
{

	launchOpenGl();

	return (0);
}



















GLFWwindow	*_windowData;
int			WWIDTH = 900;
int			WHEIGHT = 900;
char		WNAME[7] = "lem_in";

void	launchOpenGl(void)
{

	if (!glfwInit())
		return ;
		// throw std::runtime_error(RED BOLD UNDL "GLFW Initialisation Failed\n" CLR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


	GLFWmonitor	*monitor = NULL;
	
	_windowData = glfwCreateWindow(WWIDTH, WHEIGHT, WNAME, monitor, NULL);
	if (!_windowData)
	{
		glfwTerminate();
		return ;
		// throw std::runtime_error(RED BOLD UNDL "Window Creation Failed\n" CLR);
	}
	glfwMakeContextCurrent(_windowData);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return ;
		// throw std::runtime_error(RED BOLD UNDL "Glad Failed to Init\n" CLR);
	}
	
	glfwGetWindowSize(_windowData, &WWIDTH, &WHEIGHT);
	glViewport(0, 0, WWIDTH, WHEIGHT);

    // glDebugMessageCallback(opengErrorMsg, 0);

	glfwSetInputMode(_windowData, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glfwSetFramebufferSizeCallback(_windowData, _resize);
	// glfwSetKeyCallback(_windowData, _keyInput);
	// glfwSetMouseButtonCallback(_windowData, _mouseInput);

	glClearColor(0.698039216f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetCursorPos(_windowData, WWIDTH / 2.f, WHEIGHT / 2.f);
	// glfwGetCursorPos(_windowData, &_mousePos.x, &_mousePos.y);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(1); //1 = 60fps; 0 = no stop


	while(!glfwWindowShouldClose(_windowData))
	{
		glfwPollEvents();
	}


	glfwTerminate();
}






