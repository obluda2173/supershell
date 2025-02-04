/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:29:43 by erian             #+#    #+#             */
/*   Updated: 2025/02/04 16:13:16 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_quotes(char *str, size_t *i, char c)
{
	(*i)++;
	while (str[*i] != c && str[*i])
		(*i)++;
}

bool	check_consecutive_chars(char *str, char c)
{
	if (count_consequitives(str, c) > 2)
	{
		ft_putstr_fd("Parse error near ", STDERR_FILENO);
		ft_putchar_fd(c, STDERR_FILENO);
		ft_putendl_fd("", STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	check_unclosed_quotes(char *str)
{
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote)
		ft_putendl_fd("Unclosed single quote", STDERR_FILENO);
	else if (in_double_quote)
		ft_putendl_fd("Unclosed double quote", STDERR_FILENO);
	else
		return (true);
	return (false);
}

bool	check_unclosed_parenthesis(char *str, size_t open_paren)
{
	size_t	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			skip_quotes(str, &i, str[i]);
		if (str[i] == '(')
			open_paren++;
		else if (str[i] == ')')
		{
			if (open_paren == 0)
			{
				ft_putendl_fd("Unclosed parenthesis", STDERR_FILENO);
				return (false);
			}
			open_paren--;
		}
	}
	if (open_paren > 0)
	{
		ft_putendl_fd("Unclosed parenthesis", STDERR_FILENO);
		return (false);
	}
	return (true);
}
