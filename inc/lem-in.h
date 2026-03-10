/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:46:28 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 14:13:26 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "color.h"

typedef struct s_room
{
	char			*ID;
	int				nbAnts;
	struct s_room	*neighbours
}					t_rooms

#endif //LEM_IN_H
