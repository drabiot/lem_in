/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/09 21:27:13 by tchartie         ###   ########.fr       */
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

// float vertices[] = {
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
// };
// unsigned int indices[] = {  // note that we start from 0!
//     0, 1, 3,   // first triangle
//     1, 2, 3    // second triangle
// };

int	main(void)
{
	
	launchOpenGL();
	vec3 posA = GLM_VEC3_ZERO;
	vec3 posB = GLM_VEC3_ZERO;
	glm_vec3_add(posB, (vec3){0, 0, 10}, posB);
	createTunnel(10, posA, posB, 0, 0);





	printf("\n"GREEN"finish window open"BASE_COLOR"\n");

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

void	buildVerticesSmooth(float length, int sectorCount, vec_float *vertices, vec_float *normals, vec_float *texCoords, vec_int *indices, float radius)
{
	vector_clear(*vertices);
	vector_clear(*normals);
	vector_clear(*texCoords);

	vec_float	unitVertices = getUnitCircleVertices(sectorCount);

	for (int i = 0; i < 2; ++i)
	{
		float	h = -length / 2.0f + (float)i * length;
		float	t = 1.0f - (float)i;

		for (int j = 0, k = 0; j <= sectorCount; ++j, k+= 3)
		{
			float	ux = unitVertices[k];
			float	uy = unitVertices[k+1];
			float	uz = unitVertices[k+2];

			vector_add(vertices, ux * radius);
			vector_add(vertices, uy * radius);
			vector_add(vertices, h);

			vector_add(normals, ux);
			vector_add(normals, uy);
			vector_add(normals, uz);

			vector_add(texCoords, (float)j / sectorCount);
			vector_add(texCoords, t);
		}
	}

	int	baseCenterIndex = (int)vector_size(*vertices) / 3;
	int	topCenterIndex = baseCenterIndex + sectorCount + 1;

	for (int i = 0; i < 2; ++i)
	{
		float	h = -length / 2.0f + (float)i * length;
		float	nz = -1 + (float)i * 2;

		vector_add(vertices, 0);		vector_add(vertices, 0);		vector_add(vertices, h);
		vector_add(normals, 0);			vector_add(normals, 0);			vector_add(normals, nz);
		vector_add(texCoords, 0.5f);	vector_add(texCoords, 0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float	ux = unitVertices[k];
			float	uy = unitVertices[k+1];

			vector_add(vertices, ux * radius);
			vector_add(vertices, uy * radius);
			vector_add(vertices, h);

			vector_add(normals, 0);
			vector_add(normals, 0);
			vector_add(normals, nz);

			vector_add(texCoords, -ux * 0.5f + 0.5f);
			vector_add(texCoords, -uy * 0.5f + 0.5f);
		}
	}

	//Indices
	int			k1 = 0;
	int			k2 = sectorCount + 1;

	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		vector_add(indices, k1);
		vector_add(indices, k1 + 1);
		vector_add(indices, k2);

		vector_add(indices, k2);
		vector_add(indices, k1 + 1);
		vector_add(indices, k2 + 1);
	}
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			vector_add(indices, baseCenterIndex);
			vector_add(indices, k + 1);
			vector_add(indices, k);
		}
		else
		{
			vector_add(indices, baseCenterIndex);
			vector_add(indices, baseCenterIndex + 1);
			vector_add(indices, k);
		}
	}
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			vector_add(indices, topCenterIndex);
			vector_add(indices, k);
			vector_add(indices, k + 1);
		}
		else
		{
			vector_add(indices, topCenterIndex);
			vector_add(indices, k);
			vector_add(indices, topCenterIndex + 1);
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
	vec_int		indices = vector_create();

	buildVerticesSmooth(length, 12, &vertices, &normals, &texCoords, &indices, radius);

	for (unsigned long i = 0; i < vector_size(vertices) - 2 && i < vector_size(indices) - 2; i += 3)
		printf("ID: %lu, Vertices: %f, %f, %f\tIndices: %d, %d, %d\n", i / 3, vertices[i], vertices[i + 1], vertices[i + 2], indices[i], indices[i + 1], indices[i + 2]);

		
	//Execute
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// float	testv[9] = {10.f, 0.f, -5.f,
	// 				  8.660254f, 5.f, -5.f,
	// 				  10.f, 0.f, 5.f};

	// glBufferData(GL_ARRAY_BUFFER, sizeof(testv), testv, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vector_size(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// int	test[3] = {0, 1, 2};
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(test), test, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vector_size(indices), indices, GL_STATIC_DRAW);
	
	printf("\n"GREEN"finish VBO"BASE_COLOR"\n");

	shaderID	shader = createShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	if (!shader)
		return;
	t_texture	*cat = loadTexture("assets/textures/cat.bmp");
	if (!cat)
		return;

	printf("\n"GREEN"finish shader/texture"BASE_COLOR"\n");

	
	mat4	model = GLM_MAT4_IDENTITY;
	float	rangle = 0;
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (window->isRuning)
	{
		startFrame();

		if (isKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window->windowData, GLFW_TRUE);
		if (isKeyPressed(GLFW_KEY_LEFT_CONTROL))
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
		
		glm_euler_xyz((vec3){rangle, 0, 0}, model);
		rangle += GLM_PI_4 * window->deltaTime;

		setMat4(shader, "view", getView(window->camera));
		setMat4(shader, "proj", getProj(window->camera));
		setMat4(shader, "model", model);
		useTexture(cat, shader, "tex", 0);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		setVec3(shader, "color", (vec3){0, 0, 0});

		// glDrawArrays(GL_TRIANGLES, 0, sizeof(testv));
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

		setVec3(shader, "color", (vec3){1, 1, 1});
		glm_euler_xyz((vec3){rangle + GLM_PI_2, 0, 0}, model);
		setMat4(shader, "model", model);

		// glDrawElements(GL_TRIANGLES, sizeof(test), GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, vector_size(indices), GL_UNSIGNED_INT, 0);

		if (window->lockMouse)
			updateCam(window->camera);

   		endFrame();
	}
	deleteTexture(cat);
	glfwTerminate();
}






















