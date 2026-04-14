/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:16:29 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/14 13:16:29 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <cglm/cglm.h>

# define FOV 1.396263 // 80 degree in radians
# define DefaultSpeed 30.f
# define SENSI 40.f
# define FarPlane 1000.f
# define WorldUp (vec3){0, 1, 0}

typedef struct s_camera
{
	vec3	pos;
	float	yaw;
	float	pitch;
	float	aspectRatio;
	float	speed;
	vec3	dir;
	vec3	up;
	vec3	front;
	vec3	right;

	mat4	view;
	mat4	proj;
	mat4	proj2D;
}	t_camera;

t_camera	*createCam(vec3 pos, float yaw, float pitch);

float	*getPos(t_camera *camera);
void	*getView(t_camera *camera);
void	*getProj(t_camera *camera);
void	*getProj2D(t_camera *camera);

void	updateCam(t_camera *camera);

#endif