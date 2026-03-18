/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:45:50 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/18 15:50:31 by tchartie         ###   ########.fr       */
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
	if (!parseData(&farm))
		perror("ERROR");
	if (!farm.start || !farm.end)
		perror("ERROR");

	//DEBUG
	if (farm.nbAnt)
		printf("\nNb Ants: %s\n", farm.nbAnt);
	if (farm.start)
		printf("ID Start: %s\t[%d,%d]\n", farm.start->ID, farm.start->posX, farm.start->posY);
	if (farm.end)
		printf("ID End: %s\t[%d,%d]\n", farm.end->ID, farm.end->posX, farm.end->posY);

	freeFarm(&farm);
	return (0);
}
