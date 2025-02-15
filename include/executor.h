/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:42/17 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 15:42:17 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "executor_builtins.h"
# include "executor_expansions.h"
# include "libft.h"
# include "minishell.h"
# include "parser.h"
# include <dirent.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */
// executor.c
int			execute_script(t_script_node *script_node, t_data *data);

// executor_cmd_path.c
char		*get_path_env(t_list *envp);
char		*find_path(char *cmd, char *path_env);

// execute_cmd.c
int			execute_cmd_node(t_cmd_node *cmd_node, t_data *data);

/* execute_cmd_1.c */
int			custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2]);

// executor_matrix_manipulation.c
char		**ep_to_char_array(int fds[2], t_list *ep);

// executor_prepare_argv.c
char		**list_to_argv(t_list *arguments, char *cmd_path);

// executor_export_3.c
t_list		*copy_ep(t_list *ep);
bool		should_be_escaped(char c);
void		print_env_var(char *key, char *value);
void		move_invalid_keys(t_list **ep, t_list **tmp_lst, t_list **prev,
				t_list **last_node);

/* executor_helpers.c */
void		close_fds(int fds[2]);
t_env_var	*get_env_var(t_list *ep, char *key);
int			teardown_close_fds(int fds[2], char *err_msg);
int			exit_perror(char *err_msg, int exit_status);
bool		is_builtin(char *word);

/* executor_helpers_1.c */
int			is_directory(const char *path);
int			error_cmd_not_found(t_cmd_node *cn);
int			error_is_directory(char *cmd_path);

/* execute_pipeline.c */
int			execute_pipeline(t_script_node *sn, t_data *data);

#endif // EXECUTOR_
