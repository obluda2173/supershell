#ifndef LEXER_H
# define LEXER_H

typedef enum	e_token
{
	CMD,
	ARG,
	DOUBLE_QUOTE,
	QUOTE,
	REDIR_IN,
	REDIR_OUT,
	PIPE
}				t_token;

typedef struct	s_dlist
{
	char			*value;
	t_token			type;
	struct s_dlist	*previous;
	struct s_dlist	*next;
}				t_dlist;

#endif