/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:09 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/17 14:18:54 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

int	defineType(char *data)
{
	(void)data;
	return 1;
}

void	transferData(t_AntFarm *farm, char *data)
{
	static int	type = INIT;

	if (type == INIT)
		farm->nbAnt = ft_strdup(data);
	if (type == START)
		;
	if (type == END)
		;
	type = defineType(data);
}

void	fetchData(t_AntFarm *farm)
{
	char	*line;

	line = get_next_line(0);
	while (line)
	{
		transferData(farm, line);
		free(line);
		line = get_next_line(0);
	}
	free(line);
}

bool	parseData(t_AntFarm *farm)
{
	//t_room	*new_room;
	fetchData(farm);
	return (true);
}
