/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:55:09 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 15:27:42 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

void	parseData(void)
{
	char	*data;
	char	*line;

	data = ft_strdup("");
	while ((line = get_next_line(0)))
	{
		data = ft_strjoin(data, line);
		free(line);
	}
	free(data);

}
