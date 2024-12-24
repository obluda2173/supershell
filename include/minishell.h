#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>

# include "./lexer.h"
# include "./Libft/libft.h"

//structure for environment
typedef struct	s_ep
{
	char			*value;
	struct s_ep		*next;
}				t_ep;

//structure for main data
typedef struct	s_data
{
	t_ep	*ep;
	char	*line;
	bool	exit;
}				t_data;

/*
** free.c
*/
void	free_all(t_data **data);

/*
** parse.c
*/
void	parse(t_data **data);

/*
** main.c
*/
int	main(int ac, char **av, char **ep);

#endif