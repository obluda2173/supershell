/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:49:38 by erian             #+#    #+#             */
/*   Updated: 2024/08/08 10:50:49 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int chr)
{
	int	len;

	len = ft_strlen(str);
	while (len >= 0)
	{
		if (str[len] == (char)chr)
			return ((char *)str + len);
		len--;
	}
	return (NULL);
}
