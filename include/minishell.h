#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>

# include "libft.h"
# include "lexer.h"

/* ************************************************************************** */
/* structures                                                               * */
/* ************************************************************************** */

//structure for main data
typedef struct	s_data
{
	t_list			*ep;
	char			*line;
	int				exit_status;
	bool			not_exit;
}				t_data;

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

// free.c
void	free_all(t_data **data);

#endif
