/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 20:15:22 by erian             #+#    #+#             */
/*   Updated: 2024/08/05 20:30:15 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int chr, size_t nbr)
{
	const unsigned char	*s;

	s = (const unsigned char *)str;
	while (nbr--)
		if (*s++ == (unsigned char)chr)
			return ((void *)(s - 1));
	return (NULL);
}
