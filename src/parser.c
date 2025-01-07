/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:57/10 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 13:57:10 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <unistd.h>


static t_list	*create_and_add_argument(t_list **arguments, t_token *t)
{
	t_argument	*arg;
	t_list		*tmp;

	arg = extract_argument(t);
	/* if (!arg) */
	/* { */
	/* 	ft_lstclear(script, free_script_node); */
	/* 	return (NULL); */
	/* } */
	tmp = ft_lstnew(arg);
	/* if (!tmp) */
	/* { */
	/* 	ft_lstclear(script, free_script_node); */
	/* 	return (NULL); */
	/* } */
	ft_lstadd_back(arguments, tmp);
	return (tmp);
}


t_script_node *new_fill_cmd_node(t_script_node *sn, t_dllist *tokens) {
	t_token			cur;

	cur = *(t_token *)(tokens->content);
	if (cur.type != REDIRECT_OUT && cur.type != REDIRECT_IN && cur.type != REDIRECT_APPEND && cur.type != HERE_DOC)
		tokens = tokens->next;

	while (tokens)
	{
		cur = *(t_token *)(tokens->content);
		if (cur.type == PIPE)
			return sn;
		if (cur.type == END_OF_FILE)
			return sn;
		if (!create_and_add_argument(&sn->node_data.cmd_node.arguments, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return sn;
}

t_script_node	*parse(t_dllist *tokens)
{
	/* t_list	*script; */

	/* script = NULL; */

	if (!tokens)
		return create_and_add_error_node("no tokens");

	while (tokens->next) {
		if (((t_token*)tokens->content)->type == PIPE) {
			t_script_node* sn = (t_script_node*)malloc(sizeof(t_script_node));
			sn->node_type = PIPE_NODE;
			sn->num_children=2;
			sn->child1 = malloc(sizeof(t_script_node));
			if (!sn->child1) {
				free_script_node(sn);
				return create_and_add_error_node("no tokens");
			}
			sn->child2 = malloc(sizeof(t_script_node));
			if (!sn->child2) {
				free(sn->child1);
				free(sn);
				return create_and_add_error_node( "no tokens");
			}

			while (tokens->prev)
				tokens = tokens->prev;
			init_cmd_node(sn->child1, *(t_token*)tokens->content);
			new_fill_cmd_node(sn->child1, tokens);

			while (((t_token*)tokens->content)->type != PIPE)
				tokens = tokens->next;
			tokens = tokens->next;
			init_cmd_node(sn->child2, *(t_token*)tokens->content);
			new_fill_cmd_node(sn->child2, tokens);
			return sn;
		}
		tokens = tokens->next;
	}
	while (tokens->prev)
		tokens = tokens->prev;
	return (parse_cmd(tokens));
}
