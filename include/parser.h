#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "libft.h"

typedef enum {
	CMD_NODE
} t_node_type;

typedef enum {
	LITERAL,
	ENV_EXP,
	WILDCARD_EXP,
	EXIT_STATUS_EXP
} t_argument_type;

typedef enum {
	OUT,
} t_redirection_type;

typedef struct s_argument {
	char* literal;
	t_argument_type type;
} t_argument;

typedef struct s_redirection {
	char* file;
	t_redirection_type type;
} t_redirection;

typedef struct s_script_node {
	t_token cmd_token;
	t_node_type type;
	t_list* arguments;
	t_list *redirections;
} t_script_node;

t_list *parse(t_dllist *tokens);
t_token copy_token(t_token token);
void free_script_node(void *sn);

#endif // PARSER_H
