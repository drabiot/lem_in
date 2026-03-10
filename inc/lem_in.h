/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:46:28 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 16:17:39 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "color.h"
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_room
{
	char			*ID;
	int				IDAnts;
	struct s_room	*neighbours;
	int				nbNeighbours;
	bool			isUsed;
	int				posX;
	int				posY;
}					t_room;

typedef struct	s_AntFarm
{
	t_room	*room;
	t_room	*start;
	t_room	*end;
	char	*nbAnt;
}			t_AntFarm;

bool	parseData(t_AntFarm *farm);

#endif //LEM_IN_H
