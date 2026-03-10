/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchartie <tchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:08:29 by tchartie          #+#    #+#             */
/*   Updated: 2026/03/10 15:26:52 by tchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
 * See if there a newline in the string
 */

int	new_line(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (0);
}

/*
 * Join two string
 */

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ar;

	if (!s1)
		ar = (char *)malloc(sizeof(char) * (ft_strlen(s2) + 1));
	else
		ar = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!ar)
		return (NULL);
	ar = ft_strcat(s1, s2, ar);
	free(s1);
	return (ar);
}

/*
 * If check_nl == 0 : Copy the source until he see a newline to destination
 * If check_nl != 0 : Copy the source to destination
 */

char	*ft_strcpy(char *dst, const char *src, int check_nl)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (check_nl == 0)
	{
		while (src[i] != '\n' && src[i])
			dst[j++] = src[i++];
		if (src[i] == '\n')
			dst[j++] = src[i++];
	}
	else if (check_nl != 0)
	{
		while (src[i])
			dst[j++] = src[i++];
	}
	dst[i] = '\0';
	return (dst);
}

/*
 * Concatenate two string together
 */

char	*ft_strcat(char *s1, char *s2, char *array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		array[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		array[i + j] = s2[j];
		j++;
	}
	array[j + i] = '\0';
	return (array);
}
/*
 * Return a duplicate string of the given string
 */

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

/*
 * Return the size of the given string
 */

size_t	ft_strlen(const char *str)
{
	size_t	size;

	size = 0;
	if (!str)
		return (0);
	while (str[size])
		size++;
	return (size);
}
