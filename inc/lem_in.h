/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:46:28 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/19 14:54:19 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "color.h"
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_room
{
	char			*ID;
	int				IDAnts;
	struct s_room	**neighbours;
	int				nbNeighbours;
	bool			isUsed;
	int				posX;
	int				posY;
}					t_room;

typedef struct	s_AntFarm
{
	t_room	**room;
	t_room	*start;
	t_room	*end;
	char	*nbAnt;
}			t_AntFarm;

# define INIT		0
# define NO_TYPE	1
# define START		2
# define END		3
# define COMMENT	4
# define ERROR		5

# define ROOM		0
# define LINK		1

void	parseData(t_AntFarm *farm);
char	*ft_strdup(const char *src);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
int		ft_atoi(const char *str);

#endif //LEM_IN_H
