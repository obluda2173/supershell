/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax_check_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:29:43 by erian             #+#    #+#             */
/*   Updated: 2025/01/14 13:10:23 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	i;
	int	single_quote_count;
	int	double_quote_count;

	i = -1;
	single_quote_count = 0;
	double_quote_count = 0;
	while (str[++i])
	{
		if (str[i] == '\'')
			single_quote_count++;
		else if (str[i] == '\"')
			double_quote_count++;
	}
	if (single_quote_count % 2 != 0)
	{
		ft_putendl_fd("Unclosed single quote", STDERR_FILENO);
		return (false);
	}
	if (double_quote_count % 2 != 0)
	{
		ft_putendl_fd("Unclosed double quote", STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	check_invalid_symbol(char *str)
{
	if (ft_strchr(str, ';') || ft_strchr(str, '\\')
		|| ft_strchr(str, '{') || ft_strchr(str, '}')
		|| ft_strchr(str, '[') || ft_strchr(str, ']'))
	{
		ft_putendl_fd("Invalid input", STDERR_FILENO);
		return (false);
	}
	return (true);
}

static void	skip_double_quotes(char *str, size_t *i)
{
	(*i)++;
	while (str[*i] != '\"' && str[*i])
		(*i)++;
}

bool	check_unclosed_parenthesis(char *str, size_t open_paren)
{
	size_t	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\"')
			skip_double_quotes(str, &i);
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
