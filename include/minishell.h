#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>

# include "lexer.h"
# include "libft.h"

/* ************************************************************************** */
/* structures                                                               * */
/* ************************************************************************** */

/* //structure for environment */
/* typedef struct	s_ep */
/* { */
/* 	char			*value; */
/* 	struct s_ep		*next; */
/* }				t_ep; */

//structure for main data
typedef struct	s_data
{
	t_ep	*ep;
	char	*line;
	t_token_list	*tokens;
	bool	not_exit;
}				t_data;

/* ************************************************************************** */
/* parser                                                                   * */
/* ************************************************************************** */

// operators_separator.c
char	*space_line(char *line);

// parse.c
void	parse(t_data **data);

// get_tokens_1.c
void	skip_spaces(char *line, int *i);
t_token_list	*tokenize(char *line);

// get_tokens_2.c
int		identify_operator(char *line, int i);
void	link_tokens(t_token_list **head, t_token_list *current, t_token_list *prev);
t_token_list	*create_token(char *content, token_type type);
t_token_list	*create_operator_token(char *line, int *i, int op);
t_token_list	*create_command_token(char *line, int *i);

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

// free.c
void	free_tokens(t_token_list *tokens);
void	free_all(t_data **data);


#endif
