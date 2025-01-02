#ifndef LEXER_H
# define LEXER_H

#include "libft.h"
# include <stdbool.h>
# include <stdio.h>

extern const char *token_strings[];

// token types
typedef enum
{

	BUILTIN,			//0
	WORD,				//1  arbitrary chain of chars without double/single quotes or semicolons
	SINGLE_QUOTE,		//2
	DOUBLE_QUOTE,		//3
	REDIRECT_IN,		//4
	REDIRECT_OUT,		//5
	PIPE,				//6
	HERE_DOC,			//7
	REDIRECT_APPEND,	//8
	END_OF_FILE,		//9
	DOLLAR,				//10
	AND,				//11
	OR,					//12
	WILDCARD,			//13
	INVALID				//14
}	token_type;

//structure for line container
typedef struct s_line_container
{
	const char	*line;
	int			pos;
}				t_line_container;

//structure for token
typedef struct s_token
{
	char			*content;
	token_type		type;
}					t_token;

t_token	*get_next_token(t_line_container *lc);
void free_token(void *content);

/* ************************************************************************** */
/* lexer                                                                    * */
/* ************************************************************************** */

// operators_separator.c
char	*space_line(char *line);

// lexer.c
t_dllist *tokenize(char* line);

// tokenize_1.c
void	skip_spaces(char *line, int *i);

// typedef enum {
// 	EXPANSION, // $PATH
// 	LITERAL,
// 	DOUBLE_QUOTED_STRING,
// 	SINGLE_QUOTED_STRING
// }

// typedef struct s_argument {
// 	t_arg_type type;
// 	t_token token;

// }

// typedef struct s_command {
// 	t_token executable;
// 	t_argument *args;
// } t_command;

#endif
