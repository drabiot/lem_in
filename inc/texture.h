/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:36:06 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/09 18:23:34 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include <get_next_line.h>
# include <shader.h>

# include <glad/glad.h>

# define BMP_ID			0x4D42
# define HEADER_SIZE	26
# define MAX_SIZE		0x5F5E11A

typedef struct s_bmp_header
{
	uint16_t	identifier;
	uint32_t	size;
	uint32_t	reserved;
	uint32_t	data_offset;
	uint32_t	dib_size;
	uint16_t	dib_width;
	uint16_t	dib_height;
	uint16_t	dib_cp;
	uint16_t	dib_bpp;

	uint32_t	data_size;
} __attribute__((packed))	bmp_header;

typedef struct	s_texture
{
	GLuint	ID;
	char	*data;
	int		width;
	int		height;
}			t_texture;

t_texture	*loadTexture(char *path);
void		deleteTexture(t_texture	*texture);
void		useTexture(t_texture *texture, shaderID shader, char *uniform, int offset);

#endif