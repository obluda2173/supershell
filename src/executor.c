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
#include "parser.h"
#include <unistd.h>

int	execute_script(t_script_node *script_node, t_data *data)
{
	if (!script_node)
		return (0);
	if (script_node->node_type == CMD_NODE)
	{
		data->exit_status = execute_command(&script_node->node_data.cmd_node,
				data);
		return (data->exit_status);
	}
	if (script_node->downstream)
		return (execute_script(script_node->downstream, data));
	return (0);
}
