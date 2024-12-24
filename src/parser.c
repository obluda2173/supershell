/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 15:47:36 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// convert
// ls|cat -e<<her_doc>file1>>file2
// to
// ls | cat -e << her_doc > file1 >> file2

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
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

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

//handle cases when commands and separators are not separated with spaces 
static char	*space_line(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = malloc(sizeof(char) * (ft_strlen(line) + (count_separators(line) * 2) + 1));
	while (line[i])
	{
		if (line[i] == '$' && quotes(line, i) != 2 && i && line[i - 1] != '\\')
			new_line[j++] = (char)(-line[i++]);
		else if (quotes(line, i) == 0 && is_separator(line[i]))
		{
			new_line[j++] = ' ';
			new_line[j++] = line[i++];
			if (quotes(line, i) == 0 && (line[i] == '>' || line[i] == '<'))
				new_line[j++] = line[i++];
			new_line[j++] = ' ';
		}
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (new_line);
}

//main parser
void	parse(t_data **data)
{
	(*data)->line = space_line((*data)->line);
	if ((*data)->line && (*data)->line[0] == '$')
		(*data)->line[0] = (char)(-(*data)->line[0]);
	//(*data)->line = get_token((*data)->line);
	printf("%s\n", (*data)->line);
}
