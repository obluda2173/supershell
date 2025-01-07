/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2025/01/07 12:11:55 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// Skips spaces in the input string
void skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}
 
//extract allocated word from line
static char *extract_word(t_line_container *lc)
{
    char quote;
    int start;
	size_t len;
	char *word;
	
    skip_spaces((char *)lc->line, &(lc->pos));
	
	start = lc->pos;

    if (!lc->line[lc->pos])
        return NULL;

    if (ft_strchr("<>&|", lc->line[lc->pos]))
	{
		if (lc->line[lc->pos] == '<' && lc->line[lc->pos + 1] == '<' && lc->line[lc->pos + 2] != ' ')
		{
			lc->pos += 2;
			while (lc->line[lc->pos] && lc->line[lc->pos] != ' ')
				lc->pos++;
		}
        else if (lc->line[lc->pos + 1] == lc->line[lc->pos])
            lc->pos += 2;
        else
            lc->pos++;
    }
	else if (lc->line[lc->pos] >= '0' && lc->line[lc->pos] <= '2' && lc->line[lc->pos + 1] == '>' && lc->line[lc->pos + 2] != '>')
		lc->pos += 2;
	else if ((quote = lc->line[lc->pos]) == '\'' || quote == '\"')
	{
        lc->pos++; 
        while (lc->line[lc->pos] && lc->line[lc->pos] != quote)
            lc->pos++;
        lc->pos++;
    }
	else if (lc->line[lc->pos] == '$')
	{
        lc->pos++;
        while (lc->line[lc->pos] && (ft_isalnum(lc->line[lc->pos]) || lc->line[lc->pos] == '_' || lc->line[lc->pos] == '?'))
            lc->pos++;
    }
	else
        while (lc->line[lc->pos] && !ft_strchr(" \'\"<>|&$", lc->line[lc->pos]))
            lc->pos++;

    len = lc->pos - start;
	
    if (len == 0)
        return NULL;

    word = malloc(len + 1);
    if (!word)
	{
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    ft_strlcpy(word, lc->line + start, len + 1);

	printf("%s\n", word);
	
    return word;
}


bool is_builtin(char *str) {
    return (!ft_strcmp(str, "echo\0")
		|| !ft_strcmp(str, "cd\0")
		|| !ft_strcmp(str, "pwd\0")
		|| !ft_strcmp(str, "export\0")
		|| !ft_strcmp(str, "unset\0")
		|| !ft_strcmp(str, "env\0")
		|| !ft_strcmp(str, "exit\0"));
}

//returns token of given content
static token_type	assign_type(char *str)
{
	if (!str)
		return END_OF_FILE;
	if (!ft_strncmp(str, "\'", 1))
		return SINGLE_QUOTE;
	if (!ft_strncmp(str, "\"", 1))
		return DOUBLE_QUOTE;
	if (!ft_strcmp(str, "||\0"))
		return OR;
	if (!ft_strcmp(str, "&&\0"))
		return AND;
	if (!ft_strcmp(str, "=\0"))	
		return EQUAL_SIGN;
	if (!ft_strcmp(str, "|\0"))	
		return PIPE;
	if (!ft_strncmp(str, "<<", 2))
		return HERE_DOC;
	if (!ft_strcmp(str, ">>\0"))
		return REDIRECT_APPEND;
	if (!ft_strcmp(str, "<\0"))
		return REDIRECT_IN;
	if (ft_strchr(str, '>'))
		return REDIRECT_OUT;
	if (!ft_strncmp(str, "$", 1) && ft_strlen(str) > 1)
		return DOLLAR;
	if (ft_strchr(str, '*'))
		return WILDCARD;
	if (ft_strchr(str, ';') || ft_strchr(str, '\\')) //not sure
		return INVALID;
	if (is_builtin(str))
		return BUILTIN;
	return WORD;
}

// Tokenizes the input line into a doubly linked list
t_token	*get_next_token(t_line_container *lc)
{
	t_token	*token;
	char	*word;
	char	*if_dollar;

	if (!lc->line)
		return NULL;

	if (lc->line[lc->pos] == '\0')
	{
		token = malloc(sizeof(t_token));
		if (!token)
			return (NULL);
		token->content = NULL;
		token->type = END_OF_FILE;
		return (token);
	}

	word = extract_word(lc);

	token = malloc(sizeof(t_token));
	
	token->type = assign_type(word);
	if (token->type == DOLLAR && ft_strlen(word) > 1)
	{
		if_dollar = ft_substr(word, 1, ft_strlen(word));
		token->content = if_dollar;
		free(word);
	}
	else
		token->content = word;

	return (token);
}
