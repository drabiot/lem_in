/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:09 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 16:17:12 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

void	fetchData(t_AntFarm *farm)
{
	char	*data;
	char	*line;

	data = ft_strdup("");
	line = get_next_line(0);
	farm->nbAnt = line;
	while (line)
	{
		data = ft_strjoin(data, line);
		free(line);
		line = get_next_line(0);
	}
	free(line);
	//return (data);
}

bool	parseData(t_AntFarm *farm)
{
	//t_room	*new_room;
	fetchData(farm);
	return (true);
}
