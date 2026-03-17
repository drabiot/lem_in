/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:09 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/17 15:03:33 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*sent1;
	unsigned char	*sent2;

	i = 0;
	sent1 = (void *)s1;
	sent2 = (void *)s2;
	while (((sent1[i] || sent2[i]) && (sent1[i] - sent2[i] == 0)) && (n > i))
		i++;
	if (n == i)
		return (0);
	return (sent1[i] - sent2[i]);
}

int	defineType(char *data)
{
	if (!data)
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
	return (NO_TYPE);
}

void	transferData(t_AntFarm *farm, char *data)
{
	static int	type = INIT;

	if (type == INIT)
	{
		farm->nbAnt = ft_strdup(data);
		type = NO_TYPE;
		return;
	}
	if (type == START)
	{
		type = NO_TYPE;
		return;
	}
	if (type == END)
	{
		type = NO_TYPE;
		return;
	}
	type = defineType(data);
	if (type == NO_TYPE)
		;
	else if (type == COMMENT)
		;
	else if (type == ERROR)
		;
	else
		;
}

void	fetchData(t_AntFarm *farm)
{
	char	*line;
	size_t	len;

	line = get_next_line(0);
	while (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
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
