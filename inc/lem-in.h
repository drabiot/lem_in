/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:46:28 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/19 19:26:32 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "color.h"
# include <stdbool.h>
# include <stdlib.h>
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

typedef struct s_AntFarm
{
	t_room	**rooms;
	t_room	*start;
	t_room	*end;
	char	*nbAnt;
	int		inbAnt;
}			t_AntFarm;

// pathCompute.c ================================

typedef struct s_bestPred
{
	t_room				***paths;
	float				antsPerCycle;
	struct s_bestPred	*next;
}						t_bestPred;

t_room	**computePaths(t_AntFarm *antFarm);




#endif //LEM_IN_H
