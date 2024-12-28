#ifndef LEXER_H
# define LEXER_H


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

//structure for line_container
typedef struct s_line_container
{
	const char	*line;
	int			pos;
}			t_line_container;

//structure for token
typedef struct	s_token
{
	const char			*content;
	token_type		type;
}					t_token;

//structure for tokenisation
typedef struct	s_token_list
{
	t_token				*token;
	struct s_token_list	*prev;
	struct s_token_list	*next;
}						t_token_list;

#endif
