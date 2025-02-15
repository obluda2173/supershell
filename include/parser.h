/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:42/52 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 15:42:52 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "libft.h"

typedef enum e_node_type
{
	CMD_NODE,
	PIPE_NODE,
	AND_NODE,
	OR_NODE,
	ERROR_NODE,
}							t_node_type;

typedef enum e_word_type
{
	LITERAL,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	WILDCARD_EXP,
}							t_word_type;

typedef enum e_redirection_type
{
	OUT,
	IN,
	APPEND,
	HERED,
}							t_redirection_type;

typedef struct s_argument
{
	char					*word;
	t_word_type				type;
}							t_argument;

typedef struct s_redirection
{
	int						fd;
	t_redirection_type		type;
	char					*word;
	t_word_type				word_type;
}							t_redirection;

typedef struct s_error_node
{
	const char				*error;
}							t_error_node;

typedef struct s_cmd_node
{
	t_token					cmd_token;
	t_list					*arguments;
	t_list					*redirections;
}							t_cmd_node;

typedef union u_node_data
{
	t_cmd_node				cmd_node;
	t_error_node			error_node;
}							t_node_data;

typedef struct s_script_node
{
	t_node_type				node_type;
	t_node_data				node_data;
	struct s_script_node	*upstream;
	struct s_script_node	*downstream;
	unsigned int			num_children;
}							t_script_node;

/* parser.c */
t_script_node				*parse(t_dllist *tokens);

/* parse_node.c */
t_script_node				*parse_cmd(t_dllist *tokens);
t_script_node				*parse_pipe(t_dllist *tokens);
t_script_node				*parse_logical(t_dllist *tokens);

/* parser_arguments.c */
t_script_node				*create_and_add_argument(t_script_node *sn,
								t_token *t);

/* parser_free.c */
void						free_script_node(void *sn);
void						free_arguments(void *content);
void						free_redirection(void *content);

/* parser_helpers.c */
t_token						copy_token(t_token token);
bool						is_redirection_token(t_token t);

/* parser_modify_tokens.c */
t_script_node				*truncate_tokens(t_script_node *sn,
								t_dllist **tokens);
t_script_node				*set_downstream_tokens(t_dllist **downstream_tokens,
								t_dllist *tokens, t_script_node *sn);

/* parser_node_creation.c  */
t_script_node				*fill_cmd_node(t_script_node *sn, t_dllist *tokens);
t_script_node				*get_cmd_node(void);
t_script_node				*get_branch_node(t_node_type nt);
t_script_node				*get_error_node(const char *error);

/* parser_paranthesis.c */
t_script_node				*treat_parens(t_dllist *tokens);
t_script_node				*remove_next_rparen(t_dllist *downstream,
								t_script_node *sn);
bool						has_surrounding_parens(t_dllist *tokens);
t_dllist					*remove_surrounding_parens(t_dllist *tokens);

/* parser_redirections.c */
t_script_node				*create_and_add_redirection(t_dllist *head,
								t_script_node *sn);

/* parser_teardowns.c */
t_script_node				*teardown_err_in_upstream(t_script_node *sn);
t_script_node				*teardown_err_in_downstream(t_script_node *sn);
t_script_node				*teardown(t_script_node *sn, char *msg);

#endif // PARSER_H
