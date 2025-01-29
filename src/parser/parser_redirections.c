/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:49/28 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 14:49:28 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_redirection_token(t_token t)
{
	char	*content;

	content = t.content;
	while (*content != '<' && *content != '>')
		if (ft_isalpha(*(content++)))
			return (-1);
	if (t.content[0] == '>')
		return (1);
	if (t.content[0] != '>' && t.content[0] != '<')
		return (ft_atoi(t.content));
	return (0);
}

t_redirection	*parse_redirection_token(t_token t)
{
	int				fd;
	t_redirection	*r;

	fd = check_redirection_token(t);
	if (fd == -1)
		return (NULL);
	r = (t_redirection *)malloc(sizeof(t_redirection));
	if (!r)
		return (NULL);
	r->fd = fd;
	if (t.type == REDIRECT_OUT)
		r->type = OUT;
	else if (t.type == REDIRECT_IN)
		r->type = IN;
	else if (t.type == REDIRECT_APPEND)
		r->type = APPEND;
	else if (t.type == HERE_DOC)
		r->type = HERED;
	return (r);
}

t_redirection	*parse_redirection_word(t_token t, t_redirection *r)
{
	if (t.type == WORD)
		r->word_type = LITERAL;
	else if (t.type == WILDCARD)
		r->word_type = WILDCARD_EXP;
	else
	{
		free(r);
		return (NULL);
	}
	r->word = ft_strdup(t.content);
	return (r);
}

t_redirection	*extract_redirection(t_dllist *tokens)
{
	t_redirection	*r;

	r = parse_redirection_token(*(t_token *)tokens->content);
	if (!r)
		return (NULL);
	return (parse_redirection_word(*(t_token *)(tokens->next->content), r));
}

t_script_node	*create_and_add_redirection(t_dllist *head, t_script_node *sn)
{
	t_redirection	*r;
	t_list			*tmp;

	r = extract_redirection(head);
	if (!r)
	{
		free_script_node(sn);
		return (get_error_node("parsing error redirection"));
	}
	tmp = ft_lstnew(r);
	if (!tmp)
	{
		free_redirection(r);
		free_script_node(sn);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.redirections, tmp);
	return (sn);
}
