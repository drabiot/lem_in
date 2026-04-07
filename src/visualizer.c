/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:31:26 by tchartie          #+#    #+#             */
/*   Updated: 2026/04/07 23:53:42 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <visualizer.h>

void	freeFarm(t_AntFarm *farm)
{
	int	i;

	i = 0;
	while (farm->room[i])
	{
		free(farm->room[i]->ID);
		free(farm->room[i]->neighbours);
		free(farm->room[i]);
		i++;
	}
	free(farm->room);
	free(farm->nbAntstr);
}

void	freePaths(t_path **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]->path);
		free(paths[i]);
		i ++;
	}
	free(paths);
}

void	computeDistToEnd(t_room *room, int distToEnd)
{
	int	i;

	i = 0;
	room->distToEnd = distToEnd;
	while (room->neighbours[i])
	{
		if (room->neighbours[i]->distToEnd > distToEnd + 1)
			computeDistToEnd(room->neighbours[i], distToEnd + 1);
		i ++;
	}
}


int	main(void)
{
	t_AntFarm	farm;
	t_path		**paths;

	farm = (t_AntFarm){0};
	farm.room = malloc(sizeof(t_room *));
	if (!farm.room)
		return (1);
	farm.room[0] = NULL;
	parseData(&farm, true);
	if (!farm.start || !farm.end)
	{
		perror(RED"ERROR"BASE_COLOR);
		freeFarm(&farm);
		return (1);
	}


	computeDistToEnd(farm.end, 0);

	//DEBUG
	/*if (farm.nbAnt)
		printf("\nNb Ants: %s\n", farm.nbAnt);
	if (farm.start)
		printf("ID Start: %s\t\t[%d,%d]\n", farm.start->ID, farm.start->posX, farm.start->posY);
	if (farm.end)
		printf("ID End: %s\t\t[%d,%d]\n\n", farm.end->ID, farm.end->posX, farm.end->posY);

	int	i = 0;

	while (farm.room[i])
	{
		printf("ID Room [%d]: %s\t\t[%d,%d]\tnb Neighbours %d\n", i, farm.room[i]->ID, farm.room[i]->posX, farm.room[i]->posY, farm.room[i]->nbNeighbours);
		++i;
	}*/

	//Errors :
	//bad_link
	//same_ID ?????????????????


	paths = computePaths(&farm);


	////////////////////// DEBUG
	printf("\n\n");
	int	i = 0;
	int	ii = 0;
	while (paths[i])
	{
		ii = 0;
		printf("path %d found (length: %d)", i, paths[i]->nodeLenght);
		while (paths[i]->path[ii])
		{
			printf("; %s", paths[i]->path[ii]->ID);
			ii ++;
		}
		printf("\n");
		i ++;
	}
	i = 0;
	while (farm.room[i])
		i ++;
	printf("\nnbRoom: %d\n\n", i);
	//////////////////////

	if (paths)
		sim(&farm, paths);
	else
		write(1, RED"ERROR\nCould not create path\n"BASE_COLOR, 40);


	



	freePaths(paths);
	freeFarm(&farm);
	return (0);
}