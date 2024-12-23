#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include "./lexer.h"
# include "./Libft/libft.h"

typedef struct	s_shell
{
    char	*line;
	char	**ep;
	t_dlist	**command;
}				t_shell;

#endif