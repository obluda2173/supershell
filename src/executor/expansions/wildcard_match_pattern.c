/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match_pattern.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:02/02 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 16:02:02 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	matches_pattern(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (matches_pattern(pattern, str))
					return (1);
				str++;
			}
			return (0);
		}
		else if (*pattern != *str)
			return (0);
		pattern++;
		str++;
	}
	return ((!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str);
}
