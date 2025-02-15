/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_patterns.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:02:02 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 09:53:30 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_expansions.h"

char	*get_pattern(char *word)
{
	char	*pattern;
	char	*slash;

	slash = ft_strrchr(word, '/');
	if (slash)
		pattern = ft_strdup(slash + 1);
	else
		pattern = ft_strdup(word);
	return (pattern);
}

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
