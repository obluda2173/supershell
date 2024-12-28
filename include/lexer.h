#ifndef LEXER_H
# define LEXER_H


// token types
typedef enum
{
	CMD,				//1
	ARG,				//2
	SINGLE_QUOTE,		//3
	DOUBLE_QUOTE,		//4
	REDIRECT_IN,		//5
	REDIRECT_OUT,		//6
	PIPE,				//7
	HERE_DOC,			//8
	REDIRECT_APPEND,	//9
	END_OF_FILE			//10
}	token_type;

const char	*token_strings[] = {
	"CMD",				//1
	"ARG",				//2
	"'",				//3
	"\"",				//4
	"<",				//5
	">",				//6
	"|",				//7
	"<<",				//8
	">>",				//9
	"END"				//10
};

//structure for line container
typedef struct s_line_container
{
	const char	*line;
	int			pos;
}				t_line_container;

//structure for token
typedef struct s_token
{
	const char		*content;
	token_type		type;
}					t_token;

//structure for token list
typedef struct s_token_list
{
	t_token				*token;
	struct s_token_list	*prev;
	struct s_token_list	*next;
}						t_token_list;

#endif
