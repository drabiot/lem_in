/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 13:36:36 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/18 13:39:39 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "get_next_line.h"

char	*ft_strdup(const char *src)
{
	int		i;
	char	*dest;

	i = 0;
	dest = (char *)malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (dest)
	{
		while (src[i])
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
		return (dest);
	}
	return (NULL);
}

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