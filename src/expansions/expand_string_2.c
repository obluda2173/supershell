/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:11/22 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 16:11:22 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*get_min_char(char *a, char *b)
{
	if (b < a)
		return (b);
	return (a);
}

bool	is_env_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	return (false);
}

char	*advance_to_end_of_end_var(char *word)
{
	if (!word)
		return (NULL);
	if (*word == '?')
		return (word + 1);
	while (is_env_var_char(*word))
		word++;
	return (word);
}

char	*get_next_break(char *word)
{
	char	*n_double;
	char	*n_single;
	char	*n_dollar;

	n_double = ft_strchr(word, '\"');
	n_single = ft_strchr(word, '\'');
	n_dollar = ft_strchr(word, '$');
	if (n_single && n_double && n_dollar)
		return (get_min_char(get_min_char(n_double, n_single), n_dollar));
	if (n_double && n_single)
		return (get_min_char(n_double, n_single));
	if (n_double && n_dollar)
		return (get_min_char(n_double, n_dollar));
	if (n_single && n_dollar)
		return (get_min_char(n_single, n_dollar));
	if (n_double)
		return (n_double);
	if (n_single)
		return (n_single);
	if (n_dollar)
		return (n_dollar);
	return (NULL);
}
