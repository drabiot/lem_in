/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/09 16:42:59 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>
#include <math.h>

void	launchOpenGl(void);
void	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate);


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
	vec3 posA = GLM_VEC3_ZERO;
	vec3 posB = GLM_VEC3_ZERO;
	createTunnel(10, posA, posB, 0, 0);

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

/*
 * generate a unit circle on XY-plane
 *
 * Args:
 * int 			sectorCount: The total of faces to the polygon
 * 
 * Returns:
 * vec_float	unitCircleVertices:
 */
vec_float getUnitCircleVertices(int	sectorCount)
{
	float		sectorStep;
	float		sectorAngle;								// radians
	vec_float	unitCircleVertices;

	sectorStep = 2 * GLM_PI / sectorCount;
	unitCircleVertices = vector_create();
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		vector_add(&unitCircleVertices, cos(sectorAngle));	// x
		vector_add(&unitCircleVertices, sin(sectorAngle));	// y
		vector_add(&unitCircleVertices, 0);					// z
	}
	return (unitCircleVertices);
}

void	buildVerticesSmooth(float length, int sectorCount, vec_float vertices, vec_float normals, vec_float texCoords, float radius)
{
	vector_clear(vertices);
	vector_clear(normals);
	vector_clear(texCoords);

	vec_float	unitVertices = getUnitCircleVertices(sectorCount);

	for (int i = 0; i < 2; ++i)
	{
		float	h = -length / 2.0f + i * length;
		float	t = 1.0f - i;

		for (int j = 0, k = 0; j <= sectorCount; ++j, k+= 3)
		{
			float	ux = unitVertices[k];
			float	uy = unitVertices[k+1];
			float	uz = unitVertices[k+2];

			vector_add(&vertices, ux * radius);
			vector_add(&vertices, uy * radius);
			vector_add(&vertices, h);

			vector_add(&normals, ux);
			vector_add(&normals, uy);
			vector_add(&normals, uz);

			vector_add(&texCoords, (float)j / sectorCount);
			vector_add(&texCoords, t);
		}
	}

	//int	baseCenterIndex = (int)vector_size(&vertices) / 3;
	//int	topCenterIndex = baseCenterIndex + sectorCount + 1;

	for (int i = 0; i < 2; ++i)
	{
		float	h = -length / 2.0f + i * length;
		float	nz = -1 + i * 2;

		vector_add(&vertices, 0);		vector_add(&vertices, 0);		vector_add(&vertices, h);
		vector_add(&normals, 0);		vector_add(&normals, 0);		vector_add(&normals, nz);
		vector_add(&texCoords, 0.5f);	vector_add(&texCoords, 0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float	ux = unitVertices[k];
			float	uy = unitVertices[k+1];

			vector_add(&vertices, ux * radius);
			vector_add(&vertices, uy * radius);
			vector_add(&vertices, h);

			vector_add(&normals, 0);
			vector_add(&normals, 0);
			vector_add(&normals, nz);

			vector_add(&texCoords, -ux * 0.5f + 0.5f);
			vector_add(&texCoords, -uy * 0.5f + 0.5f);
		}
	}
}

void	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate)
{
	(void)radius;
	(void)angle;
	(void)rotate;

	float length;

	length = glm_vec3_distance(posA, posB);

	vec_float	vertices = vector_create();
	vec_float	normals = vector_create();
	vec_float	texCoords = vector_create();

	buildVerticesSmooth(length, 12, vertices, normals, texCoords, radius);
	
}



