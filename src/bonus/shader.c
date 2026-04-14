/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 14:09:29 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/12 17:39:44 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shader.h>

static void	freeShaderFiles(char *vshader, char *fshader)
{
	if (vshader)
		free(vshader);
	if (fshader)
		free(fshader);
}

char	*readFile(char *path)
{
	int		fd;
	char	*line;
	char	*content;

	fd = open(path, O_RDONLY);
	if (fd <= 0)
		return (NULL);
	content = ft_strdup("");
	line = get_next_line(fd);
	while (line)
	{
		content = ft_strjoin(content, line);
		line = get_next_line(fd);
	}
	close(fd);
	return (content);
}

static shaderID	loadShader(const char *data, GLenum type)
{
	int			success;
	char		infoLog[512];
	shaderID	ID;

	ID = glCreateShader(type);
	glShaderSource(ID, 1, &data, NULL);
	glCompileShader(ID);
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		write(2, data, ft_strlen(data));
		write(2, infoLog, ft_strlen(infoLog));
		return (0);
	}
	return (ID);
}

static shaderID	linkShaders(shaderID vShaderID, shaderID fShaderID)
{
	shaderID	ID;
	int			success;
	char		infoLog[512];

	ID = glCreateProgram();
	glAttachShader(ID, vShaderID);
	glAttachShader(ID, fShaderID);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		write(2, infoLog, ft_strlen(infoLog));
		return (0);
	}
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);
	return (ID);
}

shaderID	createShader(char *vShaderPath, char *fShaderPath)
{//v = vertex; f = fragment
	int		vShaderID;
	int		fShaderID;
	char	*vShader;
	char	*fShader;

	vShader = readFile(vShaderPath);
	fShader = readFile(fShaderPath);
	if (!vShader || !fShader)
	{
		freeShaderFiles(vShader, fShader);
		return (0);
	}
	vShaderID = loadShader(vShader, GL_VERTEX_SHADER);
	fShaderID = loadShader(fShader, GL_FRAGMENT_SHADER);
	// freeShaderFiles(vShader, fShader);
	if (!vShaderID || !fShaderID)
		return (0);
	return (linkShaders(vShaderID, fShaderID));
}

shaderID	deleteShader(shaderID ID)
{
	glDeleteProgram(ID);
	return (0);
}

bool	bindShader(shaderID ID)
{
	glUseProgram(ID);
	if (!ID)
		return (false);
	return (true);
}

void	setBool(shaderID ID, char *uniform, bool value)
{
	if (!ID)
		return ;
	glUniform1i(glGetUniformLocation(ID, uniform), (int)value);
}

void	setInt(shaderID ID, char *uniform, int value)
{
	if (!ID)
		return ;
	glUniform1i(glGetUniformLocation(ID, uniform), value);
}

void	setFloat(shaderID ID, char *uniform, float value)
{
	if (!ID)
		return ;
	glUniform1f(glGetUniformLocation(ID, uniform), value);
}

void	setMat4(shaderID ID, char *uniform, mat4 value)
{
	if (!ID)
		return ;
	glUniformMatrix4fv(glGetUniformLocation(ID, uniform), 1, GL_FALSE, (float *)value);
}

void	setVec3(shaderID ID, char *uniform, vec3 value)
{
	if (!ID)
		return ;
	glUniform3fv(glGetUniformLocation(ID, uniform), 1, value);
}

void	setVec2(shaderID ID, char *uniform, vec2 value)
{
	if (!ID)
		return ;
	glUniform2fv(glGetUniformLocation(ID, uniform), 1, value);
}

void	setarray(shaderID ID, char *uniform, int *value, int count)
{
	if (!ID)
		return ;
	glUniform1iv(glGetUniformLocation(ID, uniform), count, value);
}
