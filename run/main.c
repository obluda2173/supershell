/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2024/12/25 16:22:06 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//initialise data structure and extract environment
void	init(t_data **data, char **ep)
{
	t_ep	*new_node;
	t_ep	*last_node;

	//initialise all data needed
	*data = malloc(sizeof(t_data));
	if (!*data)
		return ;
	(*data)->ep = NULL;
	(*data)->exit = true;
	(*data)->tokens = NULL;

	//extract environment
	while (*ep != NULL)
	{
		new_node = ft_lstnew(*ep);
		if (!new_node)
		{
			free_all(data);
			return ;
		}
		last_node = ft_lstlast((*data)->ep);
		if (!last_node)
			(*data)->ep = new_node;
		else
			ft_lstadd_back(&((*data)->ep), new_node);
		ep++;
	}
}

//finding meeting line
char	*meeting_line(t_data **data)
{
	t_ep	*start;
	char	*line;
	char	*result;

	if (!data || !*data)
		return (NULL);
	
	start = (*data)->ep;
	line = NULL;
	result = NULL;
	
	free((*data)->line);
	(*data)->line = NULL; 
	
	while (start)
	{
		if (ft_strncmp(start->value, "LOGNAME=", 8) == 0)
		{
			line = ft_substr(start->value, 8, ft_strlen(start->value) - 8);
			if (line)
			{
				result = ft_strjoin(line, "$ ");
				free(line);
				return (result);
			}
		}
		start = start->next;
	}
	return (NULL);
}

//execution
void	execute(t_data **data)
{
	(void)data;
	//todo
}

//check syntax
bool	check_syntax(char *line)
{
	(void)line;
	return (true);
}

int	main(int ac, char **av, char **ep)
{
	t_data	*data;
	char	*read;

	//initial mandatory check
	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);
	
	data = NULL;
	read = NULL;

	//suppress warning of unused argument
	(void)av;

	//initialise data
	init(&data, ep);
	if (!data)
		return (printf("Error: Initialization failed. Exiting...\n"), 0);

	//main loop
	while (data->exit)
	{
		read = meeting_line(&data);
		data->line = readline(read);
		free(read);
		read = NULL;
		
		if (!data->line)
			break ;

		if (ft_strlen(data->line) == 0)
		{
			free(data->line);
			data->line = NULL;
			continue ;
		}
		
		if (ft_strncmp(data->line, "exit", 4) == 0)
			data->exit = false;
		
		if (check_syntax(data->line))
		{
			parse(&data);
			execute(&data);
		}
		
		free(data->line);
		data->line = NULL;
	}
	free_all(&data);
	return (0);
}
