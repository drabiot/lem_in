/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:35:54 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/09 18:39:48 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <texture.h>

static int	check_header(bmp_header *header)
{
	uint64_t	expected_size;

	if (header->identifier != BMP_ID)	
		return (0); //throw std::runtime_error(std::string("Invalid texture identifier"));
	if (header->size <= HEADER_SIZE || header->size > MAX_SIZE)	
		return (0); //throw std::runtime_error(std::string("Invalid texture size"));
	if (header->data_offset != HEADER_SIZE)	
		return (0); //throw std::runtime_error(std::string("Invalid texture data_offset"));
	if (header->dib_size != 12)	
		return (0); //throw std::runtime_error(std::string("Invalid texture dib_size"));
	header->data_size = header->size - HEADER_SIZE;
	if (header->dib_width < 5 || header->dib_height < 5)	
		return (0); //throw std::runtime_error(std::string("Invalid texture dib_width"));
	expected_size = 3 * (header->dib_height
			* (header->dib_width + header->dib_width % 4));
	if (header->data_size != expected_size)	
		return (0); //throw std::runtime_error(std::string("Invalid texture data_size"));
	if (header->dib_bpp != 24)	
		return (0); //throw std::runtime_error(std::string("Invalid texture dib_bpp"));
	if (header->dib_cp != 1)	
		return (0); //throw std::runtime_error(std::string("Invalid texture dib_cp"));
	return (1);
}

static void	readTexture(int fd, t_texture *texture)
{
	int		x;
	int		y;
	char	color[3];

	x = 0;
	y = 0;
	texture->data = malloc(sizeof(*texture->data) * texture->width
			* texture->height * 4);
	while (y < texture->height * 4)
	{
		x = 0;
		while (x < texture->width * 4)
		{
			read(fd, color, 3);
			texture->data[y * texture->width + x] = color[2];
			texture->data[y * texture->width + x + 1] = color[1];
			texture->data[y * texture->width + x + 2] = color[0];
			texture->data[y * texture->width + x + 3] = (char)255;
			x += 4;
		}
		y += 4;
	}
	close(fd);
}

t_texture	*loadTexture(char *path)
{
	int			fd;
	t_texture	*texture;
	bmp_header	header;

	fd = open(path, O_RDONLY);
	if (fd <= 0)
		return (NULL);
	if (read(fd, (char *)&header, 26) != 26 || !check_header(&header))
	{
		close(fd);
		return (NULL);
	}
	texture = malloc(sizeof(*texture));
	texture->width = header.dib_width;
	texture->height = header.dib_height;
	readTexture(fd, texture);
	glGenTextures(1, &texture->ID);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (texture);
}

void		deleteTexture(t_texture	*texture)
{
	if (!texture || !texture->ID)
		return ;
	glDeleteTextures(1, &texture->ID);
	free(texture);
	texture = NULL;
}

void		useTexture(t_texture *texture, shaderID shader, char *uniform, int offset)
{
	if (!texture || !texture->ID || !shader)
	{
		printf("%p\n", texture);
		printf("%d\n", texture->ID);
		printf("%d\n", shader);
		return ;
	}
	setInt(shader, uniform, offset);
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
}
