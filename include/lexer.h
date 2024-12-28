#ifndef LEXER_H
# define LEXER_H

typedef struct s_line_container {
	char* line;
	int pos;
} t_line_container;

// token types
typedef enum
{
	END_OF_FILE,
	CMD,				//1
	ARG,				//2
	SINGLE_QUOTE,		//3
	DOUBLE_QUOTE,		//4
	REDIRECT_IN,		//5
	REDIRECT_OUT,		//6
	PIPE,				//7
	HERE_DOC,			//8
	REDIRECT_APPEND		//9
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