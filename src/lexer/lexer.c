/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/15 13:25:30 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	free(token->content);
	free(token);
}

void	quotes(t_dllist **tokens)
{
	char		*new_content;
	t_token		*t;
	t_dllist	*tokens_lst;

	if (!tokens || !*tokens)
		return ;
	tokens_lst = *tokens;
	while (tokens_lst)
	{
		t = (t_token *)tokens_lst->content;
		if (t->type == SINGLE_QUOTE || t->type == DOUBLE_QUOTE
			|| t->type == DOLLAR)
		{
			if (t->type == SINGLE_QUOTE)
				new_content = ft_strtrim(t->content, "'");
			else if (t->type == DOUBLE_QUOTE)
				new_content = ft_strtrim(t->content, "\"");
			else if (t->type == DOLLAR)
				new_content = ft_substr(t->content, 1, ft_strlen(t->content));
			free(t->content);
			t->content = new_content;
		}
		tokens_lst = tokens_lst->next;
	}
}

t_dllist	*tokenize_line(const char *line)
{
	t_token				*token;
	t_dllist			*token_list;
	t_dllist			*new_token_node;
	t_line_container	lc;

	token = NULL;
	token_list = NULL;
	new_token_node = NULL;
	lc.line = line;
	lc.pos = 0;
	while (1)
	{
		token = get_next_token(&lc);
		if (!token)
		{
			printf("Tokenization error or empty line\n");
			return (NULL);
		}
		new_token_node = ft_dllstnew((void *)token);
		ft_dllstadd_back(&token_list, new_token_node);
		if (token->type == END_OF_FILE)
			break ;
	}
	quotes(&token_list);
	return (token_list);
}

t_dllist	*tokenize(char *line)
{
	t_dllist	*tokens;

	line = format_input(line);
	// printf("my line: %s\n", line);															//debuging
	if (!line)
		return (NULL);
	tokens = tokenize_line(line);
	free(line);
	return (tokens);
}
