/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/07 15:33:05 by erian            ###   ########.fr       */
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
	t_token		*current_token;
	t_dllist	*tokens_lst;
	char		*trimmed_content;

	if (!tokens || !*tokens)
		return ;
	tokens_lst = *tokens;
	while (tokens_lst)
	{
		current_token = (t_token *)tokens_lst->content;
		if (current_token->type == SINGLE_QUOTE
			|| current_token->type == DOUBLE_QUOTE)
		{
			if (current_token->type == SINGLE_QUOTE)
				trimmed_content = ft_strtrim(current_token->content, "'");
			else
				trimmed_content = ft_strtrim(current_token->content, "\"");
			free(current_token->content);
			current_token->content = trimmed_content;
			tokens_lst = tokens_lst->next;
		}
		else
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
	printf("\nLine: %s\n\n", line); //testing
	if (!line)
		return (NULL);
	tokens = tokenize_line(line);
	free(line);
	print_tokens(tokens); //testing
	return (tokens);
}
