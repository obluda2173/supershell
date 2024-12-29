#ifndef LEXER_H
# define LEXER_H


extern const char *token_strings[];

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
	REDIRECT_APPEND,	//8
	END_OF_FILE			//9
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
	char		*content;
	token_type		type;
}					t_token;

t_token	*get_next_token(t_line_container *lc);
void free_token(void *content);

#endif
