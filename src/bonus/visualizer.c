/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/28 13:37:07 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>


t_tunnel	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate, bool isRoom);
void		drawTunnel(t_tunnel *tunnel, shaderID shader);
void		computeTunnelModel(vec3 posA, vec3 posB, mat4 model);

vec3		otherCamPos = {0, 200, 0};
vec2		otherCamAngle = {0, -89};

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

void	switchCam(t_guiElement *elem)
{
	vec3	tpPos;
	vec2	tpAng;

	glm_vec3_add(GLM_VEC3_ZERO, window->camera->pos, tpPos);
	glm_vec3_add(GLM_VEC3_ZERO, otherCamPos, window->camera->pos);
	glm_vec3_add(GLM_VEC3_ZERO, tpPos, otherCamPos);
	tpAng[0] = window->camera->yaw;
	tpAng[1] = window->camera->pitch;
	window->camera->yaw = otherCamAngle[0];
	window->camera->pitch = otherCamAngle[1];
	otherCamAngle[0] = tpAng[0];
	otherCamAngle[1] = tpAng[1];
	updateCam(window->camera, false);

	if (!window->is2Dcam)
		setGuiText(elem, "3D Cam", true);
	else
		setGuiText(elem, "2D Cam", true);
	window->is2Dcam = !window->is2Dcam;
	fitGuiToText(elem, 1);
}

int	main(void)
{
	//Parsing
	t_AntFarm	farm;

	farm = (t_AntFarm){0};
	farm.room = malloc(sizeof(t_room *));
	if (!farm.room)
		return (1);
	farm.room[0] = NULL;
	parseData(&farm, true);
	if (!farm.start || !farm.end)
	{
		perror(RED"ERROR"BASE_COLOR);
		freeFarm(&farm);
		return (1);
	}
	

	launchOpenGL();

	initGui();
	t_guiElement	*camPosBtn = createGuiElement();
	t_guiElement	*camAngBtn = createGuiElement();
	t_guiElement	*switchCamBtn = createGuiElement();
	setGuiScale(camPosBtn, 12);
	setGuiPos(camPosBtn, (vec2){-1, 0.76});
	setGuiScale(camAngBtn, 12);
	setGuiPos(camAngBtn, (vec2){-1, 0.86});

	setGuiScale(switchCamBtn, 12);
	setGuiPos(switchCamBtn, (vec2){-1, 0.3});
	setGuiText(switchCamBtn, "2D Cam", true);
	fitGuiToText(switchCamBtn, 1);
	makeButton(switchCamBtn, switchCam);

	shaderID	shader; 
	
	shader = createShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	// LEAK?
	if (!shader)
		return (1);


	t_tunnel	*tunnels;
	int			i;
	int			j;
	int			nbTunnels;

	i = 0;
	nbTunnels = 0;
	while (farm.room[i])
	{
		j = 0;
		while (farm.room[i]->neighbours[j])
		{
			++nbTunnels;
			++j;
		}
		++i;
	}

	tunnels = malloc(sizeof(t_tunnel) * nbTunnels);
	if (!tunnels)
		return (1);

	i = 0;
	nbTunnels = 0;
	while (farm.room[i])
	{
		j = 0;
		tunnels[nbTunnels] = createTunnel(3.5,
			(vec3){farm.room[i]->posX * 20, -3, farm.room[i]->posY * 20},
			(vec3){farm.room[i]->posX * 20, 3, farm.room[i]->posY * 20},
			0, 0, true);
		++nbTunnels;
		while (farm.room[i]->neighbours[j])
		{
			if (farm.room[i]->neighbours[j]->isUsed == false)
			{
				tunnels[nbTunnels] = createTunnel(3,
					(vec3){farm.room[i]->posX * 20, 0, farm.room[i]->posY * 20},
					(vec3){farm.room[i]->neighbours[j]->posX * 20, 0, farm.room[i]->neighbours[j]->posY * 20},
					0, 0, false);
				++nbTunnels;
			}
			++j;
		}
		farm.room[i]->isUsed = true;
		++i;
	}

	while (window->isRuning)
	{
		startFrame();

		if (isKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window->windowData, GLFW_TRUE);

		if (!window->is2Dcam && isMousePressed(GLFW_MOUSE_BUTTON_MIDDLE))
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

		i = 0;
		while (i < nbTunnels)
		{
			drawTunnel(&tunnels[i], shader);
			++i;
		}

		setGuiTextf(camPosBtn, "pos: x: %.2f; y: %.2f; z: %.2f", window->camera->pos[0], window->camera->pos[1], window->camera->pos[2]);
		setGuiTextf(camAngBtn, "cam: %.2f; %.2f, speed: %d", window->camera->yaw, window->camera->pitch, (int)window->camera->speed);
		fitGuiToText(camPosBtn, 1);
		fitGuiToText(camAngBtn, 1);
		drawGui(camPosBtn);
		drawGui(camAngBtn);
		drawGui(switchCamBtn);


		if (window->lockMouse || window->is2Dcam)
       		updateCam(window->camera, true);
		if (!window->lockMouse && isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
			checkButtons();

		endFrame();
	}
	terminateGui();
	deleteShader(shader);
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

void	buildVerticesSmooth(float length, int sectorCount, int stackCount, vec_float *vertices, vec_float *normals, vec_float *texCoords, vec_int *indices, float radius, bool isRoom)
{
	vector_clear(*vertices);
	vector_clear(*normals);
	vector_clear(*texCoords);

	vec_float	unitVertices = getUnitCircleVertices(sectorCount);

	for (int i = 0; i <= stackCount; ++i)
	{
		float	t = (float)i / (float)stackCount;
		float	h = -length / 2.0f + t * length;

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
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

	for (int i = 0; i < stackCount; ++i)
	{
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			vector_add(indices, k1);
			vector_add(indices, k1 + 1);
			vector_add(indices, k2);

			vector_add(indices, k2);
			vector_add(indices, k1 + 1);
			vector_add(indices, k2 + 1);
		}
	}

	if (!isRoom)
		return ;

	int	baseCenterIndex = (int)vector_size(*vertices) / 3;
	int	topCenterIndex = baseCenterIndex + sectorCount + 1;

	for (int i = 0; i < 2; ++i)
	{
		float	h = -length / 2.0f + (float)i * length;
		float	nz = -1.0f + (float)i * 2.0f;

		vector_add(vertices, 0);		vector_add(vertices, 0);		vector_add(vertices, h);
		vector_add(normals, 0);			vector_add(normals, 0);			vector_add(normals, nz);
		vector_add(texCoords, 0.5f);	vector_add(texCoords, 0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k+1];

			vector_add(vertices, ux * radius);
			vector_add(vertices, uy * radius);
			vector_add(vertices, h);
			vector_add(normals, ux);
			vector_add(normals, uy);
			vector_add(normals, 0);
			vector_add(texCoords, -ux * 0.5f + 0.5f);
			vector_add(texCoords, -uy * 0.5f + 0.5f);
		}
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


t_tunnel	createTunnel(float radius, vec3 posA, vec3 posB, float angle, float rotate, bool isRoom)
{
	(void)angle;
	(void)rotate;

	t_tunnel	tunnel = {0};
	float 		length = glm_vec3_distance(posA, posB);

	glm_vec3_copy(posA, tunnel.posA);
	glm_vec3_copy(posB, tunnel.posB);

	vec_float	vertices = vector_create();
	vec_float	normals = vector_create();
	vec_float	texCoords = vector_create();
	vec_int		indices = vector_create();

	buildVerticesSmooth(length, 12, 100, &vertices, &normals, &texCoords, &indices, radius, isRoom);
	
	glGenVertexArrays(1, &tunnel.VAO);
	glGenBuffers(1, &tunnel.EBO);
	glBindVertexArray(tunnel.VAO);

	glGenBuffers(1, &tunnel.VVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tunnel.VVBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vector_size(vertices) * sizeof(float)), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &tunnel.NVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tunnel.NVBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vector_size(normals) * sizeof(float)), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &tunnel.UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tunnel.UVBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vector_size(texCoords) * sizeof(float)), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tunnel.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(vector_size(indices) * sizeof(int)), indices, GL_STATIC_DRAW);

	tunnel.indexCount = (int)vector_size(indices);

	glBindVertexArray(0);

	vector_free(vertices);
    vector_free(normals);
    vector_free(texCoords);
    vector_free(indices);

	return (tunnel);
}

void	drawTunnel(t_tunnel *tunnel, shaderID shader)
{
	mat4	model;
	computeTunnelModel(tunnel->posA, tunnel->posB, model);

	setMat4(shader, "view",  getView(window->camera));
    setMat4(shader, "proj",  getProj(window->camera));
    setMat4(shader, "model", model);
    //useTexture(tunnel->texture, shader, "tex", 0);

    glBindVertexArray(tunnel->VAO);
    glDrawElements(GL_TRIANGLES, tunnel->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void	computeTunnelModel(vec3 posA, vec3 posB, mat4 model)
{
	glm_mat4_identity(model);

	vec3 center;
	glm_vec3_add(posA, posB, center);
	glm_vec3_scale(center, 0.5f, center);
	glm_translate(model, center);

	vec3 dir;
	glm_vec3_sub(posB, posA, dir);
	glm_vec3_normalize(dir);

	vec3 zAxis = {0, 0, 1};
	vec3 axis;
	glm_vec3_cross(zAxis, dir, axis);

	float dot = glm_vec3_dot(zAxis, dir);
	float angle = acosf(dot);

	if (glm_vec3_norm(axis) < 0.001f)
	{
		if (dot < 0)
			glm_rotate(model, GLM_PI, (vec3){1, 0, 0});
	}
	else
	{
		glm_vec3_normalize(axis);
		glm_rotate(model, angle, axis);
	}
}