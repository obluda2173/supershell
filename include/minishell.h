#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

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
void	free_data(t_data *data);

t_data	*init(char **ep);
char	*meeting_line(t_data **data);
char * rl_gets(const char* prompt);

#endif
