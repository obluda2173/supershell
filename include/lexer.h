/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:58:41 by erian             #+#    #+#             */
/*   Updated: 2025/01/16 20:24:42 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>

extern const char	*token_strings[];

typedef enum e_token_type
{
	BUILTIN,			// 0
	WORD,				// 1
	SINGLE_QUOTE,		// 2
	DOUBLE_QUOTE,		// 3
	REDIRECT_IN,		// 4
	REDIRECT_OUT,		// 5
	PIPE,				// 6
	HERE_DOC,			// 7
	REDIRECT_APPEND,	// 8
	END_OF_FILE,		// 9
	DOLLAR,				// 10
	AND,				// 11
	OR,					// 12
	WILDCARD,			// 13
	EQUAL_SIGN,			// 14
	LPAREN,				// 15
	RPAREN,				// 16
	INVALID,			// 17
	NONE				/* added by Kay to have a null-value for t_token_type*/
}	t_token_type;

typedef struct s_line_container
{
	const char	*line;
	int			pos;
}	t_line_container;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
}	t_token;

/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */

// lexer_format_input_1.c
char		*format_input(char *line);

// lexer_format_input_2.c
char		*handle_redirection(char *input, size_t *i, char *result, size_t *j);
char		*handle_heredoc(char *input, size_t *i, char *result, size_t *j);
char		*handle_quotes(const char *input, size_t *i, char *result, size_t *j);
char		*handle_operators(const char *input, size_t *i, char *result, size_t *j);

// syntax_check_1.c
int		count_consequitives(char *str, char c);
bool	check_syntax(char *str);

// lexer_syntax_check_2.c
bool	check_consecutive_chars(char *str, char c);
bool	check_unclosed_quotes(char *str);
bool	check_invalid_symbol(char *str);
bool	check_unclosed_parenthesis(char *str, size_t open_paren);

// lexer.c
void		free_token(void *content);
t_dllist	*tokenize(char *line);

// lexer_tokenize_1.c
t_token		*create_token(char *content, t_token_type type);
t_token		*get_next_token(t_line_container *lc);

// lexer_tokenize_2.c
void		skip_operator(t_line_container *lc);
void		skip_quoted_text(t_line_container *lc);
void		skip_variable(t_line_container *lc);
void		skip_unquoted_word(t_line_container *lc);
char		*allocate_word(const char *line, int start, size_t len);

// lexer_tokenize_3.c
t_token_type assign_type(char *str);

#endif
