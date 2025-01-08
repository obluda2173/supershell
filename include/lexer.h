/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:58:41 by erian             #+#    #+#             */
/*   Updated: 2025/01/08 14:40:46 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

#include "libft.h"
# include <stdbool.h>
# include <stdio.h>

extern const char *token_strings[];

// token types
typedef enum
{
	
	BUILTIN,			//0
	WORD,				//1  arbitrary chain of chars without double/single quotes or semicolons
	SINGLE_QUOTE,		//2
	DOUBLE_QUOTE,		//3
	REDIRECT_IN,		//4
	REDIRECT_OUT,		//5
	PIPE,				//6
	HERE_DOC,			//7
	REDIRECT_APPEND,	//8
	END_OF_FILE,		//9
	DOLLAR,				//10
	AND,				//11
	OR,					//12
	WILDCARD,			//13
	EQUAL_SIGN,			//14
	INVALID				//15
}	token_type;

//structure for line container
typedef struct s_line_container
{
	const char	*line;
	int			pos;
}				t_line_container;

//structure for token
typedef struct s_token
{
	char			*content;
	token_type		type;
}					t_token;

/* ************************************************************************** */
/* lexer                                                                    * */
/* ************************************************************************** */

// debuging.c
void		print_tokens(t_dllist *head);

// format_input_1.c
char		*format_input(char *line);

// format_input_2.c
char		*handle_redirection(char *input, size_t *i, char *result, size_t *j);
char		*handle_heredoc(char *input, size_t *i, char *result, size_t *j);
char		*handle_quotes(const char *input, size_t *i,
							char *result, size_t *j);
char		*handle_operators(const char *input, size_t *i,
								char *result, size_t *j);

// heredoc.c
int heredoc_loop(t_dllist **tokens);

// lexer.c
void		free_token(void *content);
t_dllist	*tokenize(char* line);

// tokenize_1.c
t_token		*get_next_token(t_line_container *lc);

// tokenize_2.c
void		skip_operator(t_line_container *lc);
void		skip_quoted_text(t_line_container *lc);
void		skip_variable(t_line_container *lc);
void		skip_unquoted_word(t_line_container *lc);
char		*allocate_word(const char *line, int start, size_t len);

// tokenize_3.c
token_type	assign_type(char *str);

#endif
