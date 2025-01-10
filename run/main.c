/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/10 10:36:58 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "executor.h"

//initialise data structure and extract environment
void	init(t_data **data, char **ep)
{
	t_list	*new_node;
	t_list	*last_node;

	//initialise all data needed
	*data = malloc(sizeof(t_data));
	if (!*data)
		return ;
	(*data)->ep = NULL;
	(*data)->not_exit = true;

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
	t_list	*start;
	char	*line;
	char	*result;

	if (!data || !*data)
		return (NULL);

	start = (*data)->ep;
	line = NULL;
	result = NULL;
	while (start)
	{
		if (ft_strncmp(start->content, "LOGNAME=", 8) == 0)
		{
			line = ft_substr(start->content, 8, ft_strlen(start->content) - 8);
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
	while (data->not_exit)
	{
		read = meeting_line(&data);
		data->line = readline(read);
		free(read);
		read = NULL;

		if (!data->line)
			break ;

		//empty line handler
		if (ft_strlen(data->line) == 0)
		{
			free(data->line);
			data->line = NULL;
			continue ;
		}

		//exit the process
		if (ft_strncmp(data->line, "exit", 4) == 0)
		{
			data->not_exit = false;
			continue ;
		}

		if (!check_syntax(data->line))
		{
			free(data->line);
			data->line = NULL;
			continue ;
		}

		t_dllist *tokens = tokenize(data->line);
		if (!heredoc_loop(&tokens))
		{
			printf("Error: Heredoc processing failed.\n");
			ft_dllstclear(&tokens, free_token);
			continue ;
		}

		t_script_node *script = parse(tokens);
		execute_script(script, ep);
		
		ft_dllstclear(&tokens, free_token);
		free_script_node(script);

		free(data->line);
		data->line = NULL;
	}
	free_all(&data);
	return (0);
}
