/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:58:41 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 13:48:08 by erian            ###   ########.fr       */
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
	REDIRECT_IN,		// 2
	REDIRECT_OUT,		// 3
	PIPE,				// 4
	HERE_DOC,			// 5
	REDIRECT_APPEND,	// 6
	END_OF_FILE,		// 7
	AND,				// 8
	OR,					// 9
	WILDCARD,			// 10
	LPAREN,				// 11
	RPAREN,				// 12
	INVALID,			// 13
	NONE
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
char			*format_input(char *line);

// lexer_format_input_2.c
char			*handle_redirection(char *input, size_t *i,
					char *result, size_t *j);
char			*handle_heredoc(char *input, size_t *i,
					char *result, size_t *j);
char			*handle_quotes(const char *input, size_t *i,
					char *result, size_t *j);
char			*handle_operators(const char *input, size_t *i,
					char *result, size_t *j);

// lexer_syntax_check_1.c
int				count_consequitives(char *str, char c);
bool			check_syntax(char *str);

// lexer_syntax_check_2.c
bool			check_consecutive_chars(char *str, char c);
bool			check_unclosed_quotes(char *str);
bool			check_invalid_symbol(char *str);
bool			check_unclosed_parenthesis(char *str, size_t open_paren);

// lexer_tokenize_1.c
t_token			*create_token(char *content, t_token_type type);
t_token			*get_next_token(t_line_container *lc);

// lexer_tokenize_2.c
void			skip_operator(t_line_container *lc);
void			skip_word(t_line_container *lc);
char			*allocate_word(const char *line, int start, size_t len);

// lexer_tokenize_3.c
t_token_type	assign_type(char *str);

// lexer.c
void			free_token(void *content);
t_dllist		*tokenize(char *line);

#endif
