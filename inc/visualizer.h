/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 23:49:24 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/21 13:40:24 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISUALIZER_H
# define VISUALIZER_H

# include <openGLMagic.h>
# include <texture.h>
# include <gui.h>
# include <vec.h>
# include <math.h>
# include <lem_in.h>

typedef	float	*vec_float;
typedef	int		*vec_int;

typedef struct s_tunnel
{
	vec3			posA;
	vec3			posB;
	unsigned int	VAO;
	unsigned int	VBO;
	unsigned int	EBO;
	int				indexCount;
	t_texture		*texture;
}					t_tunnel;

#endif