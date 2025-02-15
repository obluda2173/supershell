/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:23:59 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 10:39:51 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->ep)
		ft_lstclear(&(data->ep), free_env_var);
	if (data->line)
		free(data->line);
	free(data);
}

static void	update_shlvl(t_list *ep)
{
	t_env_var	*shlvl;
	int			sh_nbr;

	shlvl = get_env_var(ep, "SHLVL");
	if (!shlvl)
	{
		shlvl = new_env_var("SHLVL", "1");
		ft_lstadd_back(&ep, ft_lstnew(shlvl));
		return ;
	}
	if (!shlvl->value)
		shlvl->value = ft_strdup("0");
	sh_nbr = ft_atoi(shlvl->value) + 1;
	if (sh_nbr >= 1000)
	{
		printf("minishell: warning: shell level ");
		printf("(%d) too high, resetting to 1\n", sh_nbr);
		sh_nbr = 1;
	}
	free(shlvl->value);
	shlvl->value = ft_itoa(sh_nbr);
}

// initialise data structure and extract environment
t_data	*init(char **ep)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->ep = NULL;
	data->exit_status = 0;
	data->exit = false;
	data->line = NULL;
	if (!set_env_vars(ep, data))
		return (NULL);
	update_shlvl(data->ep);
	return (data);
}
