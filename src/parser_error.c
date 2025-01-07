/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:33/51 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/03 09:33:51 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_script_node	*get_error_node(const char *error)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	sn->node_type = ERROR_NODE;
	sn->node_data.error_node.error = error;
	return (sn);
}
