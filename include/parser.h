/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:23/33 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/03 09:23:33 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "libft.h"

typedef enum e_node_type
{
	CMD_NODE,
	ERROR_NODE,
}						t_node_type;

typedef enum e_argument_type
{
	LITERAL,
	ENV_EXP,
	WILDCARD_EXP,
	EXIT_STATUS_EXP
}						t_argument_type;

typedef enum e_redirection_type
{
	OUT,
	IN,
	APPEND,
	HERED,
}						t_redirection_type;

typedef struct s_argument
{
	char				*literal;
	t_argument_type		type;
}						t_argument;

typedef struct s_redirection
{
	char				*file;
	t_redirection_type	type;
}						t_redirection;

typedef struct s_error_node
{
	const char			*error;
}						t_error_node;

typedef struct s_cmd_node
{
	t_token				cmd_token;
	t_list				*arguments;
	t_list				*redirections;
}						t_cmd_node;

typedef union u_node_data
{
	t_cmd_node			cmd_node;
	t_error_node		error_node;
}						t_node_data;

typedef struct s_script_node
{
	t_node_type			node_type;
	t_node_data			node_data;
}						t_script_node;

void					init_cmd_node(t_script_node *sn, t_token t);
t_list					*parse(t_dllist *tokens);
t_token					copy_token(t_token token);
void					free_script_node(void *sn);

/* parser cmd */
t_list	*parse_cmd(t_list *script, t_dllist *tokens);
t_redirection			*extract_redirection(t_dllist *tokens);
t_argument				*extract_argument(t_token *t);

/* node creation */
t_script_node	*create_and_add_error_node(t_list **script, const char *error);

/* free */
void					free_arguments(void *content);
void					free_redirection(void *content);
void					free_script_node(void *sn);
t_token					copy_token(t_token token);


#endif // PARSER_H
