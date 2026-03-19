/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:45:50 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/19 19:34:59 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem-in.h>

t_room	*roomMaker(char *ID)
{
	t_room	*newRoom;
	
	newRoom = malloc(sizeof(t_room));
	if (!newRoom)
		return (NULL);
	newRoom->ID = ID;
	newRoom->IDAnts = 0;
	newRoom->neighbours = malloc(sizeof(t_room));
	newRoom->neighbours[0] = NULL;
	newRoom->nbNeighbours = 0;
	newRoom->isUsed = false;
	newRoom->posX = 0;
	newRoom->posY = 0;
	return (newRoom);
}

void	addNeighbour(t_room *room, t_room *newNeighbour)
{
	t_room	**newNeighboursList;
	int		i;

	newNeighboursList = malloc(sizeof(t_room) * (room->nbNeighbours + 2));
	newNeighboursList[room->nbNeighbours + 1] = NULL;
	i = 0;
	while (i < room->nbNeighbours)
	{
		newNeighboursList[i] = room->neighbours[i];
		i ++;
	}
	newNeighboursList[room->nbNeighbours] = newNeighbour;
	room->nbNeighbours += 1;
	free(room->neighbours);
	room->neighbours = newNeighboursList;
}

int	main(void)
{
	t_AntFarm	antFarm;
	
	antFarm.start = roomMaker("0");
	antFarm.end = roomMaker("3");

	antFarm.rooms = malloc(sizeof(t_room) * 5);
	antFarm.rooms[4] = NULL;
	antFarm.rooms[0] = antFarm.start;
	antFarm.rooms[1] = roomMaker("1");
	antFarm.rooms[2] = roomMaker("2");
	antFarm.rooms[3] = antFarm.end;
	antFarm.inbAnt = 10;

	addNeighbour(antFarm.rooms[0], antFarm.rooms[1]);
	addNeighbour(antFarm.rooms[1], antFarm.rooms[2]);
	addNeighbour(antFarm.rooms[2], antFarm.rooms[3]);


	t_room * tp0 = roomMaker("0.5");
	addNeighbour(antFarm.rooms[0], tp0);
	addNeighbour(tp0, antFarm.rooms[0]);

	t_room * tp1 = roomMaker("1.5");
	addNeighbour(antFarm.rooms[1], tp1);
	addNeighbour(tp1, antFarm.rooms[2]);
	addNeighbour(tp0, tp1);
	addNeighbour(tp1, tp0);

	t_room * tp2 = roomMaker("2.5");
	addNeighbour(antFarm.rooms[3], tp2);
	addNeighbour(tp2, antFarm.rooms[3]);
	addNeighbour(tp1, tp2);
	addNeighbour(tp2, tp1);



	computePaths(&antFarm);



	return (0);
}
