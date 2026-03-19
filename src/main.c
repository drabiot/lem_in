/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:45:50 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/19 14:13:39 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	freeFarm(t_AntFarm *farm)
{
	int	i;

	i = 0;
	while (farm->room[i])
	{
		free(farm->room[i]->ID);
		free(farm->room[i]);
		i++;
	}
	free(farm->room);
	free(farm->nbAnt);
}

int	main(void)
{
	t_AntFarm	farm;

	farm = (t_AntFarm){0};
	farm.room = malloc(sizeof(t_room *));
	if (!farm.room)
		return (1);
	farm.room[0] = NULL;
	if (!parseData(&farm) || !farm.start || !farm.end)
	{
		perror(RED"ERROR"BASE_COLOR);
		freeFarm(&farm);
		return (1);
	}

	//DEBUG
	if (farm.nbAnt)
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
	}

	//Errors :
	//bad_link
	//same_ID ?????????????????
	//room_long
	//room_short

	freeFarm(&farm);
	return (0);
}
