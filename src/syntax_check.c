/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 09:40:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/04 12:35:36 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
