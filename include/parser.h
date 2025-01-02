#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "libft.h"

typedef enum {
	CMD_NODE,
	ERROR_NODE,
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

typedef struct s_error_node {
	const char* error;
} t_error_node;

typedef struct s_cmd_node {
	t_token cmd_token;
	t_list* arguments;
	t_list *redirections;
} t_cmd_node;

typedef union u_node_data {
	t_cmd_node cmd_node;
	t_error_node error_node;
} t_node_data;

typedef struct s_script_node {
	t_node_type node_type;
	t_node_data node_data;
} t_script_node;

void	init_cmd_node(t_script_node *sn, t_token t);
t_list *parse(t_dllist *tokens);
t_token copy_token(t_token token);
void free_script_node(void *sn);

/* extraction */
t_redirection	*extract_redirection(t_dllist *tokens);
t_argument	*extract_argument(t_token *t);

/* free */
void	free_arguments(void *content);
void	free_redirection(void *content);
void	free_script_node(void *sn);
t_token	copy_token(t_token token);

#endif // PARSER_H
