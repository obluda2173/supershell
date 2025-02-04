/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtins.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:42:30 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/04 10:27:13 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_BUILTINS_H
# define EXECUTOR_BUILTINS_H

# include "minishell.h"
# include "parser.h"

/* executor/builtins */
/* echo.c */
int		echo(t_cmd_node cmd_node, int fds[2]);

// builtin_cd_1.c
int		cstm_cd(t_list **ep, t_list *args);

// builtin_pwd.c
int		cstm_pwd(t_list **ep, t_cmd_node *cmd_node);

// builtin_exit.c
int		cstm_exit(t_list *args, t_data *data);

// builtin_cd_2.c
void	update_dirs(t_list **ep);
int		check_args(t_list *args);

// executor_unset.c
int		cstm_unset(t_list **ep, t_cmd_node *cmd_node);

// executor_env.c
int		cstm_env(t_list **ep, t_cmd_node *cmd_node);

// executor_export_1.c
int		cstm_export(t_list **ep, t_cmd_node *cmd_node);

// executor_export_2.c
bool	assign_var(t_env_var **new_var, char *raw_var);
#endif // EXECUTOR_BUITLINS_H
