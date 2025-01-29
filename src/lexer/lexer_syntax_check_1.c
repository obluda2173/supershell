/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_syntax_check_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 09:40:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 12:46:13 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_consequitives(char *str, char c)
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


/*

" just 'a simpl"e and mere text written" by 'mr' Erik' An"

*/

bool	check_inv_char(char *str)
{
	int	i;
	int d_quotes;
	int s_quotes;

	i = -1;
	d_quotes = 0;
	s_quotes = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && (d_quotes % 2 == 0))
			s_quotes++;
		if (str[i] == '\"' && (s_quotes % 2 == 0))
			d_quotes++;
		if ((s_quotes % 2 == 0) && (d_quotes % 2 == 0) &&
			(str[i] == '\\' || str[i] == ';'))
		{
			if (str[i] == ';')
				ft_putendl_fd("syntax error near unexpected token: \';\'", STDERR_FILENO);
			else
				ft_putendl_fd("syntax error near unexpected token: \'\\\'", STDERR_FILENO);
			return (false);
		}
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
	if (!check_unclosed_parenthesis(str, 0))
		return (false);
	if (!check_inv_char(str))
		return (false);
	return (true);
}
