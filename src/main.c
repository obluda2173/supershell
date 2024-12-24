/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 11:46:26 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>



void	init(t_data **data, char **ep)
{
	t_ep	*node;
	//initialise all data needed
	*data = malloc(sizeof(t_data));
    if (!*data)
        return ;
    (*data)->ep = NULL;

	//extract environment
	while (*ep != NULL)
	{
		node = ft_lstnew(*ep);
		ft_lstadd_back(&((*data)->ep), node);
		ep++;
	}
}

int main(int ac, char **av, char **ep)
{
	t_data	*data;
	//initial mandatory check
	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);
		
	//suppress warning of unused argument
	(void)av;

	//initialise data
	init(&data, ep);
	
	t_ep *current = data->ep;
	while (current)
    {
        printf("%s\n", current->value);
        current = current->next;
    }
	
	return (0);
}
