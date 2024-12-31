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

typedef struct s_argument {
	char* literal;
	t_argument_type type;
} t_argument;

typedef struct s_script_node {
	t_token token;
	t_node_type type;
	t_argument** arguments;
	int argument_count;
} t_script_node;

t_list *parse(t_dllist *tokens);
t_token copy_token(t_token token);
void free_script_node(void *sn);

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

#endif // PARSER_H
