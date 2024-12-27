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

//structure for environment
typedef struct	s_ep
{
	char			*value;
	struct s_ep		*next;
}				t_ep;

// token types
typedef enum
{
	CMD,				//0
	ARG,				//1
	SINGLE_QUOTE,		//2
	DOUBLE_QUOTE,		//3
	REDIRECT_IN,		//4
	REDIRECT_OUT,		//5
	PIPE,				//6
	HERE_DOC,			//7
	REDIRECT_APPEND		//8
}	token_type;			

//structure for tokenisation
typedef struct	s_token
{
	char			*content;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

//structure for main data
typedef struct	s_data
{
	t_ep	*ep;
	char	*line;
	t_token	*tokens;
	bool	exit;
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
t_token	*get_token(char *line);

// get_tokens_2.c
int		identify_operator(char *line, int i);
void	link_tokens(t_token **head, t_token *current, t_token *prev);
t_token	*create_token(char *content, token_type type);
t_token	*create_operator_token(char *line, int *i, int op);
t_token	*create_command_token(char *line, int *i);

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

// free.c
void	free_tokens(t_token *tokens);
void	free_all(t_data **data);


#endif
