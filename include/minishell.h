#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
#include <sys/wait.h>
# include "libft.h"
# include "lexer.h"

/* ************************************************************************** */
/* global variables                                                               * */
/* ************************************************************************** */

#include <signal.h>
extern volatile sig_atomic_t	signal_received;

/* ************************************************************************** */
/* structures                                                               * */
/* ************************************************************************** */

typedef struct s_env_var {
	char* key;
	char* value;
} t_env_var;

//structure for main data
typedef struct	s_data
{
	t_list			*ep;		/* list of t_env_var */
	char			*line;
	int				exit_status;
	bool			exit;
}				t_data;

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

int	repl(t_data *data);
char	*minishell_input(t_data *data);
// free.c
void	free_data(t_data *data);

t_data	*init(char **ep);
void	handle_signals(int signum);
char	*read_line_from_fd(int read_fd);

// heredoc.c
int			heredoc_loop(t_dllist **tokens, t_data *data);
char	*read_line_from_fd(int read_fd);

t_env_var *new_env_var(char *key, char *value);
void free_env_var(void *content);

int	error_fork(void);
void	handle_signals_2(int signum);
#endif
