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
	t_list			*ep;
	char			*line;
	size_t			i;
	t_dllist		*tokens;
	bool			not_exit;
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
t_token	*get_next_token(t_line_container *lc);


/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

// free.c
//void	free_tokens(t_dllist *tokens);
//void	free_all(t_data **data);


#endif
