/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:16:18 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/21 13:25:28 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <camera.h>
# include <openGLMagic.h>

t_camera	*createCam(vec3 pos, float yaw, float pitch)
{
	t_camera	*camera;

	camera = malloc(sizeof(*camera));
	glm_vec3_add(GLM_VEC3_ZERO, pos, camera->pos);
	camera->yaw = yaw;
	camera->pitch = pitch;
	camera->aspectRatio = 0;
	camera->speed = DefaultSpeed;
	glm_vec3_zero(camera->dir);
	glm_vec3_add(GLM_VEC3_ZERO, WorldUp, camera->up);
	glm_vec3_zero(camera->front);
	glm_vec3_zero(camera->right);

	return (camera);
}

float	*getPos(t_camera *camera)
{
	return (camera->pos);
}

void	*getView(t_camera *camera)
{
	return (camera->view);
}

void	*getProj(t_camera *camera)
{
	return (camera->proj);
}

void	*getProj2D(t_camera *camera)
{
	return (camera->proj2D);
}

static void	setClampVal(float *var, float val, float min, float max)
{
	if (val < min)
		val = min;
	if (val > max)
		val = max;
	*var = val;
}

static void	updateMatrices(t_camera *camera)
{
	vec3	center;
	glm_vec3_add(camera->pos, camera->front, center);
	glm_lookat(camera->pos, center, WorldUp, camera->view);
	glm_perspective(FOV, camera->aspectRatio, 0.1f, FarPlane, camera->proj);
	glm_ortho(0., window->width, 0, window->height, 0.1f, 100, camera->proj2D);
}

void	updateCam(t_camera *camera, bool takeInput)
{
	// updating camera orientation
	if (takeInput && !window->is2Dcam)
	{
		camera->yaw = fmodf(camera->yaw + SENSI
			* (getMousePosX() - (getWindowW() / 2.))
			/ getWindowW(), 360);
		setClampVal(&camera->pitch, camera->pitch - SENSI
			* (getMousePosY() - (getWindowH() / 2.))
			/ getWindowH(), -89, 89);
	}
	camera->dir[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	camera->dir[1] = sin(glm_rad(camera->pitch));
	camera->dir[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	glm_vec3_normalize_to(camera->dir, camera->front);
	glm_vec3_crossn(camera->front, WorldUp, camera->right);
	glm_vec3_crossn(camera->right, camera->front, camera->up);
	// updating camera pos
	float	speedChange = (getMouseScrollY() + isKeyPressed(GLFW_KEY_KP_ADD)
		- isKeyPressed(GLFW_KEY_KP_SUBTRACT)) * 10;
	if (camera->speed + speedChange > 100)
		speedChange *= 10;
	setClampVal(&camera->speed, camera->speed + speedChange, 10, 5000);
	if (takeInput)
	{
		if (isKeyRepeated(GLFW_KEY_W) && !window->is2Dcam)
			glm_vec3_muladds(camera->dir, -camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_S) && !window->is2Dcam)
			glm_vec3_muladds(camera->dir, camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_A))
			glm_vec3_muladds(camera->right, camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_D))
			glm_vec3_muladds(camera->right, -camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_SPACE))
			glm_vec3_muladds(WorldUp, -camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_LEFT_SHIFT))
			glm_vec3_muladds(WorldUp, camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_W) && window->is2Dcam)
			glm_vec3_muladds((vec3){1, 0, 0}, -camera->speed * window->deltaTime, camera->pos);
		if (isKeyRepeated(GLFW_KEY_S) && window->is2Dcam)
			glm_vec3_muladds((vec3){1, 0, 0}, camera->speed * window->deltaTime, camera->pos);
	}
	if (window->is2Dcam)
		setClampVal(&camera->pos[1], camera->pos[1], Min2DHeight, FarPlane);
	// now we update the matrices with everything
	updateMatrices(camera);
}


