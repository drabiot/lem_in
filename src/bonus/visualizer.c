/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/14 14:00:04 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>


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

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int	main(void)
{
	launchOpenGL();

	initGui();

	t_guiElement	*camPos = createGuiElement();
	t_guiElement	*camAng = createGuiElement();
	setGuiScale(camPos, 12);
	setGuiPos(camPos, (vec2){-1, 0.76});
	setGuiScale(camAng, 12);
	setGuiPos(camAng, (vec2){-1, 0.86});

	printf("\n"GREEN"finish window open"BASE_COLOR"\n");

	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO1 = 0;
	glGenBuffers(1, &VBO1);
	unsigned int VBO2 = 0;
	glGenBuffers(1, &VBO2);
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	printf("\n"GREEN"finish VBO"BASE_COLOR"\n");

	shaderID	shader = createShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	if (!shader)
		return (0);
	t_texture	*cat = loadTexture("assets/textures/cat.bmp");
	if (!cat)
		return (0);

	printf("\n"GREEN"finish shader/texture"BASE_COLOR"\n");

	while (window->isRuning)
	{
		startFrame();

		if (isKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window->windowData, GLFW_TRUE);
		if (isMousePressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			window->lockMouse = !window->lockMouse;
			if (window->lockMouse)
			{
				glfwSetCursorPos(window->windowData, window->width / 2, window->height / 2);
				glfwSetInputMode(window->windowData, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				refreshMouse();
			}
			else
				glfwSetInputMode(window->windowData, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		bindShader(shader);
		


		setMat4(shader, "view", getView(window->camera));
		setMat4(shader, "proj", getProj(window->camera));
		setMat4(shader, "model", GLM_MAT4_IDENTITY);
		useTexture(cat, shader, "tex", 0);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		setGuiTextf(camPos, "pos: x: %.2f; y: %.2f; z: %.2f", window->camera->pos[0], window->camera->pos[1], window->camera->pos[2]);
		setGuiTextf(camAng, "cam: %.2f; %.2f, speed: %d", window->camera->yaw, window->camera->pitch, (int)window->camera->speed);
		fitGuiToText(camPos, 1);
		fitGuiToText(camAng, 1);

		drawGui(camPos);
		drawGui(camAng);


		if (window->lockMouse)
			updateCam(window->camera);

   		endFrame();
	}
	deleteTexture(cat);
	terminateGui();
	glfwTerminate();
	return (0);
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






















