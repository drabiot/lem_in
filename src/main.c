/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:45:50 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 16:19:10 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int	main(void)
{
	t_AntFarm	farm;

	farm = (t_AntFarm){0};
	if (!parseData(&farm))
		perror("ERROR");
	return (0);
}
