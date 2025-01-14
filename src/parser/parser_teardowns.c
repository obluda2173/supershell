/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_teardowns.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:00/13 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/14 11:00:13 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_script_node	*teardown_err_in_upstream(t_script_node *sn)
{
	t_script_node	*err_node;

	err_node = sn->upstream;
	free_script_node(sn->downstream);
	free(sn);
	return (err_node);
}

t_script_node	*teardown_err_in_downstream(t_script_node *sn)
{
	t_script_node	*err_node;

	err_node = sn->downstream;
	free(sn);
	return (err_node);
}

t_script_node	*teardown(t_script_node *sn, char *msg)
{
	free_script_node(sn);
	return (get_error_node(msg));
}
