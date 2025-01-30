/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 09:40:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/30 16:12:20 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	in_quotes(char *str, int i)
{
	int	j;
	int	d;
	int	s;
	
	j = 0;
	d = 0;
	s = 0;
	while (j < i)
	{
		if (str[j] == '\'' && (d % 2 == 0))
			s++;
		if (str[j] == '\"' && (s % 2 == 0))
			d++;
		j++;
	}
	if ((s % 2 == 0) && (d % 2 == 0))
		return (false);
	return (true);
}

static bool	check_invalid_symbol(char *str)
{
	int		i;
	char	*inv_char;

	i = -1;
	inv_char = "[]{}\\;";
	while (str[++i])
	{
		if (ft_strchr(inv_char, str[i]) && !in_quotes(str, i))
		{
			ft_putendl_fd("Invalid input", STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}

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
		if (str[i] == c && !in_quotes(str, i))
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
	return (true);
}
