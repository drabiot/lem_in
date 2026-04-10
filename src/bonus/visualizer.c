/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/10 14:13:25 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>


t_tunnel	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate);
void		drawTunnel(t_tunnel *tunnel, mat4 model);


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

    vec3 posA1 = GLM_VEC3_ZERO;
    vec3 posB1 = {0, 0, 10};
    t_tunnel t1 = createTunnel(10, posA1, posB1, 0, 0);

    vec3 posA2 = {0, 0, 10};
    vec3 posB2 = {0, 0, 20};
    t_tunnel t2 = createTunnel(20, posA2, posB2, 0, 0);

	mat4 model = GLM_MAT4_IDENTITY_INIT;

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

		drawTunnel(&t1, model);
		drawTunnel(&t2, model);

		if (window->lockMouse)
        updateCam(window->camera);

		endFrame();
	}
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

t_tunnel	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate)
{
	(void)angle;
	(void)rotate;

	t_tunnel	tunnel = {0};
	float 		length = glm_vec3_distance(posA, posB);

	vec_float	vertices = vector_create();
	vec_float	normals = vector_create();
	vec_float	texCoords = vector_create();
	vec_int		indices = vector_create();

	buildVerticesSmooth(length, 12, &vertices, &normals, &texCoords, &indices, radius);
	
	glGenVertexArrays(1, &tunnel.VAO);
	glGenBuffers(1, &tunnel.VBO);
	glGenBuffers(1, &tunnel.EBO);
	glBindVertexArray(tunnel.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, tunnel.VBO);

	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vector_size(vertices) * sizeof(float)), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tunnel.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(vector_size(indices) * sizeof(int)), indices, GL_STATIC_DRAW);

	tunnel.indexCount = (int)vector_size(indices);
	tunnel.shader = createShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	//if (!tunnel.shader)
	//	return (NULL);

	glBindVertexArray(0);

	vector_free(vertices);
    vector_free(normals);
    vector_free(texCoords);
    vector_free(indices);

	return (tunnel);
}

void	drawTunnel(t_tunnel *tunnel, mat4 model)
{
	bindShader(tunnel->shader);
	setMat4(tunnel->shader, "view",  getView(window->camera));
    setMat4(tunnel->shader, "proj",  getProj(window->camera));
    setMat4(tunnel->shader, "model", model);
    //useTexture(tunnel->texture, tunnel->shader, "tex", 0);

    glBindVertexArray(tunnel->VAO);
    glDrawElements(GL_TRIANGLES, tunnel->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

