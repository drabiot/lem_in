/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 22:42:57 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/07 23:03:58 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>




















void	sim(t_AntFarm *antFarm, t_path **paths)
{
	int	startAnts;
	int	endAnts;
	int	i;
	int	ii;

	startAnts = antFarm->nbAnt;
	endAnts = 0;

	while (endAnts != antFarm->nbAnt)
	{
		i = 0;
		while (paths[i])
		{
			ii = 1;
			while (paths[i]->path[ii])
			{
				if (paths[i]->path[ii]->IDAnts)
				{
					printf("L%d-%s ", paths[i]->path[ii]->IDAnts, paths[i]->path[ii - 1]->ID);
					if (ii > 1)
						paths[i]->path[ii - 1]->IDAnts = paths[i]->path[ii]->IDAnts;
					else
						endAnts ++;
					paths[i]->path[ii]->IDAnts = 0;
				}
				ii ++;
			}
			if (startAnts && (startAnts >= paths[i]->nodeLenght || i == 0))
			{
				paths[i]->path[paths[i]->nodeLenght]->IDAnts = antFarm->nbAnt - startAnts + 1;
				printf("L%d-%s ", antFarm->nbAnt - startAnts + 1, paths[i]->path[paths[i]->nodeLenght]->ID);
				startAnts --;
			}
			i ++;
		}
		printf("\n");
	}

}


/*

2
1 0 2
##start
0 2 0
##end
4 2 6
2 4 2
3 4 4
0-1
0-2
2-3
3-4
4-1

*/