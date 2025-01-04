/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators_separator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:33:18 by erian             #+#    #+#             */
/*   Updated: 2025/01/04 12:22:11 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

What does this file do?

it converts:
ls|cat -e<<her_doc>file1>>file2

to
ls | cat -e << her_doc > file1 >> file2

*/

//quotes counter
static int	quotes(char *line, int i)
{
	int	j;
	int	double_quotes_count;
	int	single_quotes_count;

	j = 0;
	double_quotes_count = 0;
	single_quotes_count = 0;
	while (j < i)
	{
		if (line[j] == 34) // 34 == " 
			double_quotes_count++;
		else if (line[j] == 39) // 39 == '
			single_quotes_count++;
		j++;
	}
	if (single_quotes_count % 2 == 1 && double_quotes_count % 2 == 0)
		return (1); //if we inside single quotes
	else if (double_quotes_count % 2 == 1 && single_quotes_count % 2 == 0)
		return (2); //if we inside double quotes
	else if (single_quotes_count % 2 == 1 && double_quotes_count % 2 == 1)
		return (3);	//if we inside single and double quotes
	else
		return (0); //if we outside of any quotes
}

//not sure if there are any other separators
static bool	is_separator(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (true);
	return (false);
}

//counts the number of operators in the line passed
static int	count_separators(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (is_separator(line[i]))
			count++;
		i++;
	}
	return (count);
}

static bool	is_fd_redirection(char *line, int i)
{
	if (ft_isdigit(line[i]) && line[i + 1] == '>' && (line[i + 2] == '&' || line[i + 2] == '-'))
		return (true);
	return (false);
}

static bool	is_logical_operator(char *line, int i)
{
	if (line[i] == '&' && line[i + 1] == '&')
		return (true);
	if (line[i] == '|' && line[i + 1] == '|')
		return (true);
	return (false);
}

char	*space_line(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = malloc(sizeof(char) * (ft_strlen(line) + (count_separators(line) * 2) + 1));
	if (!new_line)
		return (NULL);
	while (line[i])
	{
		// Handle file descriptor redirection
		if (is_fd_redirection(line, i))
		{
			new_line[j++] = ' ';
			while (isdigit(line[i]) || line[i] == '>' || line[i] == '&' || line[i] == '-')
				new_line[j++] = line[i++];
			new_line[j++] = ' ';
		}
		// Handle logical operators
		else if (is_logical_operator(line, i))
		{
			new_line[j++] = ' ';
			new_line[j++] = line[i++];
			new_line[j++] = line[i++];
			new_line[j++] = ' ';
		}
		// Handle separators outside quotes
		else if (quotes(line, i) == 0 && is_separator(line[i]))
		{
			new_line[j++] = ' ';
			new_line[j++] = line[i++];
			if (quotes(line, i) == 0 && (line[i] == '>' || line[i] == '<' || line[i] == '|' || line[i] == '&'))
				new_line[j++] = line[i++];
			new_line[j++] = ' ';
		}
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	printf("%s\n", new_line);
	return (new_line);
}
