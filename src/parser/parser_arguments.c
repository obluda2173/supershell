/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_arguments.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:47/52 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 14:47:52 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_argument	*extract_argument(t_token *t)
{
	t_argument	*arg;

	arg = (t_argument *)malloc(sizeof(t_argument));
	if (!arg)
		return (NULL);
	arg->word = ft_strdup(t->content);
	if (t->type == WORD || t->type == BUILTIN)
		arg->type = LITERAL;
	if (t->type == WILDCARD)
		arg->type = WILDCARD_EXP;
	return (arg);
}

t_script_node	*create_and_add_argument(t_script_node *sn, t_token *t)
{
	t_argument	*arg;
	t_list		*tmp;

	arg = extract_argument(t);
	if (!arg)
	{
		free_script_node(sn);
		return (NULL);
	}
	tmp = ft_lstnew(arg);
	if (!tmp)
	{
		free_script_node(sn);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.arguments, tmp);
	return (sn);
}
