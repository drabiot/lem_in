/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 14:05:21 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/14 13:08:10 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_H
# define SHADER_H

# include <get_next_line.h>
char	*ft_strdup(const char *src); //!!TODO!! waiting for libft

# include <stdbool.h>

# include <cglm/cglm.h>
# include <glad/glad.h>

typedef unsigned int shaderID;

shaderID	createShader(char *vShaderPath, char *fShaderPath);
shaderID	deleteShader(shaderID ID);
bool		bindShader(shaderID ID);

void	setBool(shaderID ID, char *uniform, bool value);
void	setInt(shaderID ID, char *uniform, int value);
void	setFloat(shaderID ID, char *uniform, float value);
void	setMat4(shaderID ID, char *uniform, mat4 value);
void	setVec3(shaderID ID, char *uniform, vec3 value);
void	setVec2(shaderID ID, char *uniform, vec2 value);
void	setarray(shaderID ID, char *uniform, int *value, int count);

#endif