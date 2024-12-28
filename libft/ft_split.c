/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:44:19 by erian             #+#    #+#             */
/*   Updated: 2024/08/06 15:43:00 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count_char(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s && *s != c)
	{
		count++;
		s++;
	}
	return (count);
}

size_t	ft_count_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	char	**start;
	size_t	len;

	str = malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!str || !s)
		return (NULL);
	start = str;
	while (*s)
	{
		if (*s != c)
		{
			len = ft_count_char(s, c);
			*str = malloc((len + 1) * sizeof(char));
			if (!*str)
				return (NULL);
			ft_strlcpy(*str, s, len + 1);
			str++;
			s += len;
		}
		else
			s++;
	}
	*str = NULL;
	return (start);
}
