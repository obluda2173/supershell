/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 20:42:34 by erian             #+#    #+#             */
/*   Updated: 2024/08/05 21:12:26 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*s2)
		return ((char *)s1);
	i = 0;
	while (s1[i] && i < len)
	{
		j = 0;
		while ((s1[i + j] == s2[j]) && (i + j < len))
		{
			if (!s2[j + 1])
				return ((char *)(s1 + i));
			j++;
		}
		i++;
	}
	return (NULL);
}
