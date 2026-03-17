/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:45:50 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/17 15:03:51 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int	main(void)
{
	t_AntFarm	farm;

	farm = (t_AntFarm){0};
	if (!parseData(&farm))
		perror("ERROR");

	printf("\nNb Ants: %s\n", farm.nbAnt);

	return (0);
}
