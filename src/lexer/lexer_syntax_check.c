/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 09:40:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/13 13:35:08 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_consequitives(char *str, char c)
{
	int	c_counter;
	int	counter;
	int	i;
	int	j;

	c_counter = 0;
	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			j = i;
			while (str[j] == c)
			{
				counter++;
				j++;
			}
			if (c_counter < counter)
				c_counter = counter;
			counter = 0;
		}
		i++;
	}
	return (c_counter);
}

static bool	check_consecutive_chars(char *str, char c)
{
	if (count_consequitives(str, c) > 2)
	{
		printf("Parse error near '%c'\n", c);
		return (false);
	}
	return (true);
}

static bool	check_unclosed_quotes(char *str)
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
		printf("Unclosed single quote\n");
		return (false);
	}
	if (double_quote_count % 2 != 0)
	{
		printf("Unclosed double quote\n");
		return (false);
	}
	return (true);
}

static bool	check_invalid_symbol(char *str)
{
	if (ft_strchr(str, ';') || ft_strchr(str, '\\')
		|| ft_strchr(str, '{') || ft_strchr(str, '}')
		|| ft_strchr(str, '[') || ft_strchr(str, ']'))
	{
		printf("Invalid input\n");
		return (false);
	}
	return (true);
}

bool	check_syntax(char *str)
{
	int		i;
	char	special_chars[4];

	i = -1;
	special_chars[0] = '<';
	special_chars[1] = '>';
	special_chars[2] = '|';
	special_chars[3] = '&';
	if (!check_unclosed_quotes(str))
		return (false);
	while (++i < 4)
		if (!check_consecutive_chars(str, special_chars[i]))
			return (false);
	if (!check_invalid_symbol(str))
		return (false);
	return (true);
}
