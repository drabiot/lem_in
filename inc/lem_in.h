/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:46:28 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 15:08:10 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "color.h"
# include <stdbool.h>
# include <unistd.h>

typedef struct s_room
{
	char			*ID;
	int				IDAnts;
	struct s_room	*neighbours;
	bool			isUsed;
	int				posX;
	int				posY;
}					t_rooms;

void	parseData(void);

#endif //LEM_IN_H
