/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:43:00 by erian             #+#    #+#             */
/*   Updated: 2024/08/05 19:47:48 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int chr)
{
	while (*str && *str != (char)chr)
		str++;
	if (*str != (char)chr)
		return (NULL);
	return ((char *)str);
}
