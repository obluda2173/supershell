/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:01/54 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 14:01:54 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"

void	init_cmd_node(t_script_node *sn, t_token t)
{
	sn->node_data.cmd_node.cmd_token = copy_token(t);
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
}

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
	return (r);
}

t_redirection	*parse_redirection_word(t_token t, t_redirection *r)
{
	if (t.type == WORD)
		r->word_type = LITERAL;
	else if (t.type == DOLLAR)
		r->word_type = ENV_EXP;
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

t_argument	*extract_argument(t_token *t)
{
	t_argument	*arg;

	arg = (t_argument *)malloc(sizeof(t_argument));
	if (!arg)
		return (NULL);
	arg->word = ft_strdup(t->content);
	if (t->type == WORD || t->type == DOUBLE_QUOTE || t->type == SINGLE_QUOTE)
		arg->type = LITERAL;
	if (t->type == DOLLAR)
	{
		arg->type = ENV_EXP;
		if (!ft_strncmp(arg->word, "?", 1))
			arg->type = EXIT_STATUS_EXP;
	}
	if (t->type == WILDCARD)
		arg->type = WILDCARD_EXP;
	return (arg);
}
