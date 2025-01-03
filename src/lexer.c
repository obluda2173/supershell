/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/03 11:43:23 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

const char *token_strings[] = {
    "BUILTIN",
    "WORD",
    "'",
    "\"",
    "<",
    ">",
    "|",
    "<<",
    ">>",
    "END",
    "$",
    "&&",
    "||",
    "*",
    "INV"
};

void	free_token(void *content)
{
	t_token *token;

	token = (t_token *)content;
	free(token->content);
	free(token);
}

//function to print the content of doubly list of tokens
static void print_tokens(t_dllist *head)
{
	t_dllist *current;
	t_token *token;

	if (!head)
	{
		printf("The token list is empty.\n");
		return;
	}

	current = head;
	printf("Tokens in the doubly linked list:\n");
	while (current && current->content)
	{
		if (!current->content)
		{
			printf("Corrupted token node detected.\n");
			break;
		}
		token = (t_token *)current->content;
		if (token)
		{
			printf("Content: %s, Type: %s\n", token->content, token_strings[token->type]);
		}
		current = current->next;
	}
}

void ft_dllstinsert_before(t_dllist *node, t_dllist *new_node)
{
	if (!node || !new_node)
		return;

	new_node->next = node;
	new_node->prev = node->prev;

	if (node->prev)
		node->prev->next = new_node;
	node->prev = new_node;
}

void	quotes(t_dllist **tokens)
{
	t_dllist *tokens_lst;
	t_dllist *new_node;
	char	*combined_content = NULL;

	if (!tokens || !*tokens)
		return;

	tokens_lst = *tokens;

	while (tokens_lst)
	{
		t_token *current_token = (t_token *)tokens_lst->content;

		if (current_token && current_token->type == DOUBLE_QUOTE)
		{
			tokens_lst = tokens_lst->next; // Skip opening DOUBLE_QUOTE
			while (tokens_lst)
			{
				t_token *inner_token = (t_token *)tokens_lst->content;

				// End of DOUBLE_QUOTE section
				if (inner_token && inner_token->type == DOUBLE_QUOTE)
				{
					t_dllist *to_free = tokens_lst;
					tokens_lst = tokens_lst->next; // Remove closing DOUBLE_QUOTE
					if (to_free->prev)
						to_free->prev->next = to_free->next;
					if (to_free->next)
						to_free->next->prev = to_free->prev;
					free_token(to_free->content);
					free(to_free);
					break;
				}

				// Handle DOLLAR as a separator
				if (inner_token && inner_token->type == DOLLAR)
				{
					// Create a new node for the combined content so far
					if (combined_content)
					{
						t_token *new_token = malloc(sizeof(t_token));
						new_token->type = WORD;
						new_token->content = combined_content;
						new_node = ft_dllstnew(new_token);
						ft_dllstinsert_before(tokens_lst, new_node);
						combined_content = NULL;
					}
					tokens_lst = tokens_lst->next;
					continue;
				}

				// Concatenate non-DOLLAR tokens into combined_content
				if (inner_token && inner_token->type != DOLLAR)
				{
					char *new_combined = ft_strjoin(combined_content ? combined_content : "", inner_token->content);
					free(combined_content);
					combined_content = new_combined;
				}

				// Remove the processed token
				t_dllist *to_free = tokens_lst;
				tokens_lst = tokens_lst->next;
				if (to_free->prev)
					to_free->prev->next = to_free->next;
				if (to_free->next)
					to_free->next->prev = to_free->prev;
				free_token(to_free->content);
				free(to_free);
			}

			// Create a final node for remaining combined content
			if (combined_content)
			{
				t_token *new_token = malloc(sizeof(t_token));
				new_token->type = WORD;
				new_token->content = combined_content;
				new_node = ft_dllstnew(new_token);
				ft_dllstinsert_before(tokens_lst, new_node);
				combined_content = NULL;
			}
		}
		//if see token with type SINGLE_QUOTE trim the '' from both sides
		else if (current_token->type == SINGLE_QUOTE)
		{
			char *trimmed_content = ft_strtrim(current_token->content, "'");
			free(current_token->content);
			current_token->content = trimmed_content;
			tokens_lst = tokens_lst->next;
		}
		else
			tokens_lst = tokens_lst->next;
	}
}

t_dllist *tokenize_line(const char *line)
{
	t_line_container lc = {line, 0};
	t_dllist *token_list = NULL;
	t_token *token = NULL;
	t_dllist *new_token_node = NULL;

	while (1)
	{
		token = get_next_token(&lc);
		if (!token)
		{
			printf("Tokenization error or empty line\n");
			return NULL;
		}

		new_token_node = ft_dllstnew((void *)token);
		if (!new_token_node)
		{
			printf("Memory allocation failed for token node\n");
			free(token);
			return NULL;
		}
		ft_dllstadd_back(&token_list, new_token_node);
		if (token->type == END_OF_FILE)
			break ;
	}

	quotes(&token_list);
	
	return token_list;
}


t_dllist *tokenize(char* line)
{
	line = space_line(line);
	if (!line)
		return NULL;

	t_dllist *tokens = tokenize_line(line);
	free(line);
	
	
	print_tokens(tokens);
	return tokens;
}
