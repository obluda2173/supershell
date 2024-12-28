#ifndef LEXER_H
# define LEXER_H

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

const char	*token_strings[] = {
	"CMD",
	"ARG",
	"'",
	"\"",
	"<",
	">",
	"|",
	"<<",
	">>"
};

//structure for tokenisation
typedef struct	s_token
{
	char			*content;
	token_type		type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

#endif