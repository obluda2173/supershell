/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:42/45 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 15:42:45 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>

typedef enum e_token_type
{
	BUILTIN,
	WORD,
	REDIRECT_IN,
	REDIRECT_OUT,
	PIPE,
	HERE_DOC,
	REDIRECT_APPEND,
	END_OF_FILE,
	AND,
	OR,
	WILDCARD,
	LPAREN,
	RPAREN,
	INVALID,
	HD_SINGLE_QUOTE,
	HD_DOUBLE_QUOTE,
	NONE
}					t_token_type;

typedef struct s_line_container
{
	const char		*line;
	int				pos;
}					t_line_container;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
}					t_token;

/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */

// lexer_format_input_1.c
char				*format_input(char *line);

// lexer_format_input_2.c
char				*handle_redirection(char *input, size_t *i, char *result,
						size_t *j);
char				*handle_heredoc(char *input, size_t *i, char *result,
						size_t *j);
char				*handle_quotes(const char *input, size_t *i, char *result,
						size_t *j);
char				*handle_operators(const char *input, size_t *i,
						char *result, size_t *j);

// lexer_syntax_check_1.c
int					count_consequitives(char *str, char c);
bool				check_syntax(char *str);

// lexer_syntax_check_2.c
bool				check_consecutive_chars(char *str, char c);
bool				check_unclosed_quotes(char *str);
bool				check_invalid_symbol(char *str);
bool				check_unclosed_parenthesis(char *str, size_t open_paren);

// lexer_tokenize_1.c
t_token				*create_token(char *content, t_token_type type);
t_token				*get_next_token(t_line_container *lc);

// lexer_tokenize_2.c
void				skip_operator(t_line_container *lc);
void				skip_word(t_line_container *lc);
char				*allocate_word(const char *line, int start, size_t len);

// lexer_tokenize_3.c
t_token_type		assign_type(char *str);

// lexer.c
void				free_token(void *content);
t_dllist			*tokenize(char *line);

#endif
