/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:36:00 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 10:53:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <sys/wait.h>
#include <unistd.h>

int	execute_script(t_script_node *sn, t_data *data)
{
	int	exit_upstream;

	if (!sn)
		return (0);
	if (sn->node_type == CMD_NODE)
		return (execute_command(&sn->node_data.cmd_node, data));
	if (sn->node_type == PIPE_NODE)
		return (execute_pipeline(sn, data));
	if (sn->node_type == AND_NODE)
	{
		exit_upstream = execute_script(sn->upstream, data);
		if (exit_upstream)
			return (exit_upstream);
		return (execute_script(sn->downstream, data));
	}
	if (sn->node_type == OR_NODE)
	{
		exit_upstream = execute_script(sn->upstream, data);
		if (!exit_upstream)
			return (exit_upstream);
		return (execute_script(sn->downstream, data));
	}
	return (EXIT_SUCCESS);
}
