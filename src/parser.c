/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:09 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/19 13:22:15 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

static void	freeSplit(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		++i;
	}
	free(split);
}

static bool	isDigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		++i;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		++i;
	}
	return (true);
}

t_room	*createRoom(char *data)
{
	t_room	*room;
	char	**split;

	room = malloc(sizeof(t_room));
	if (!room)
		return (NULL);
	split = ft_split(data, ' ');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
	{
		free(room);
		freeSplit(split);
		return (NULL);
	}
	if (!isDigit(split[1]) || !isDigit(split[2]))
	{
		free(room);
		freeSplit(split);
		return (NULL);
	}
	room->ID = ft_strdup(split[0]);
	room->posX = ft_atoi(split[1]);
	room->posY = ft_atoi(split[2]);
	room->IDAnts = 0;
	room->neighbours = NULL;
	room->nbNeighbours = 0;
	room->isUsed = false;
	freeSplit(split);
	return (room);
}

/*bool	linkRoom(t_AntFarm *farm, char *data)
{
	char	**split;

	split = ft_split(data, '-');
	if (!split || !split[0] || !split[1])
	{
		freeSplit(split);
		return (false);
	}
	
}*/

void	addRoomToFarm(t_AntFarm *farm, t_room *room)
{
	t_room	**new;
	int		i;

	i = 0;
	while (farm->room[i])
		i++;
	new = malloc(sizeof(t_room *) * (i + 2));
	if (!new)
		return ;
	i = 0;
	while (farm->room[i])
	{
		new[i] = farm->room[i];
		i++;
	}
	new[i] = room;
	new[i + 1] = NULL;
	free(farm->room);
	farm->room = new;
}

int	defineType(char *data)
{
	if (!data || !data[0] || data[0] == 'L')
		return (ERROR);
	if (data[0] == '#' && data[1] == '#')
	{
		char *after = data + 2;
		while (*after == ' ' || *after == '\t')
			after++;
		if (ft_strncmp(after, "start", 6) == 0)
			return (START);
		if (ft_strncmp(after, "end", 4) == 0)
			return (END);
	}
	else if (data[0] == '#')
		return (COMMENT);
	return (NO_TYPE);
}

int	checkType(char *data)
{
	char	**split;

	split = ft_split(data, ' ');
	if (!split)
	{
		freeSplit(split);
		return (ERROR);
	}
	if (split[0] && !split[1])
	{
		freeSplit(split);
		return (LINK);
	}
	else
	{
		freeSplit(split);
		return (ROOM);
	}
}

bool	transferData(t_AntFarm *farm, char *data)
{
	static int	type = INIT;
	static int	defType = INIT;
	t_room		*new_room;

	if (type == INIT)
	{
		farm->nbAnt = ft_strdup(data);
		type = NO_TYPE;
		return (true);
	}
	if (type == START || type == END)
	{
		new_room = createRoom(data);
		if (!new_room)
			return (false);
		if (type == START)
			farm->start = new_room;
		else
			farm->end = new_room;
		addRoomToFarm(farm, new_room);
		type = NO_TYPE;
		return (true);
	}
	type = defineType(data);
	if (type == NO_TYPE)
	{
		defType = checkType(data);
		if (defType == ERROR)
			return (false);
		if (defType == ROOM)
		{
			new_room = createRoom(data);
			addRoomToFarm(farm, new_room);
		}
		if (defType == LINK)
			;
		return (true);
	}
	else if (type == COMMENT)
		return (true);
	else if (type == ERROR)
		return (false);
	return (true);
}

bool	fetchData(t_AntFarm *farm)
{
	char	*line;
	size_t	len;

	line = get_next_line(0);
	while (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (!transferData(farm, line))
		{
			free(line);
			get_next_line(-1);
			return (false);
		}
		free(line);
		line = get_next_line(0);
	}
	free(line);
	return (true);
}

bool	parseData(t_AntFarm *farm)
{
	if (!fetchData(farm))
		return (false);
	return (true);
}
