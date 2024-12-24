#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include "./lexer.h"
# include "./Libft/libft.h"

typedef struct	s_ep
{
	char			*value;
	struct s_ep		*next;
}				t_ep;

typedef struct	s_data
{
	t_ep	*ep;
}				t_data;

#endif