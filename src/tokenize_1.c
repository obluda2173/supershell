/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 13:05:17 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Skips spaces in the input string
void skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}

char	*extract_word(t_data **data)
{
		//todo
}

// Tokenizes the input line into a doubly linked list
t_token_list	*tokenize(t_data **data)
{
	int		*i;
	char	*line;
	t_token	*token;

	token = malloc(sizeof(t_token));
	
	i = (*data)->i;
	line = 
	
	while ((*data)->i < ft_strlen((*data)->line) && (*data)->line[i] != ' ')
	{
		
	}
}
