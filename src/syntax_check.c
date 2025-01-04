/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 09:40:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/04 13:21:02 by erian            ###   ########.fr       */
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

bool	check_syntax(char *str)
{
	int	double_quote_count;
	int	single_quote_count;
	
	int	i;

	double_quote_count = 0;
	single_quote_count = 0;
	i = -1;
	while(str[++i])
	{
		if (str[i] == '\'')
			single_quote_count++;
		else if (str[i] == '\"')
			double_quote_count++;
	}

	//check for <<< and etc
	if (count_consequitives(str, '<') > 2)
	{
		printf("Parse error near '<'\n");
		return (false);
	}
	
	if (count_consequitives(str, '>') > 2)
	{
		printf("Parse error near '>'\n");
		return (false);
	}

	if (count_consequitives(str, '|') > 2)
	{
		printf("Parse error near '|'\n");
		return (false);
	}

	if (count_consequitives(str, '&') > 2)
	{
		printf("Parse error near '&'\n");
		return (false);
	}

	if (single_quote_count % 2 != 0)
	{
		printf("Unclosed single quote\n");
		return (false);
	}
	else if (double_quote_count % 2 != 0)
	{
		printf("Unclosed double quote\n");
		return (false);
	}
	return (true);
}
