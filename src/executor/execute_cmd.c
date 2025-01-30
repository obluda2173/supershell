/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 16:21:14 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_builtins.h"
#include "libft.h"
#include <unistd.h>

int	expand(t_cmd_node *cmd_node, t_data *data)
{
	if (expand_redirections(cmd_node, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (expand_arguments(cmd_node, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	execute_builtin(t_cmd_node *cmd_node, t_data *data, int fds[2])
{
	if (!ft_strcmp("echo", cmd_node->cmd_token.content))
		return (echo(*cmd_node, fds));
	if (!ft_strcmp("export", cmd_node->cmd_token.content))
		return (cstm_export(&data->ep, cmd_node));
	if (!ft_strcmp("unset", cmd_node->cmd_token.content))
		return (cstm_unset(&data->ep, cmd_node));
	if (!ft_strcmp("env", cmd_node->cmd_token.content))
		return (cstm_env(&data->ep, cmd_node));
	if (!ft_strcmp("pwd", cmd_node->cmd_token.content))
		return (cstm_pwd(&data->ep, cmd_node));
	if (!ft_strcmp("cd", cmd_node->cmd_token.content))
		return (cstm_cd(&data->ep, cmd_node->arguments));
	if (!ft_strcmp("exit", cmd_node->cmd_token.content))
		return (cstm_exit(cmd_node->arguments, data));
	return (EXIT_SUCCESS);
}

int	execute_cmd(t_cmd_node *cmd_node, t_data *data, int fds[2])
{
	char	*path_env;
	char	*cmd_path;
	int		res;
	char	**argv;

	argv = NULL;
	path_env = get_path_env(data->ep);
	cmd_path = find_path(cmd_node->cmd_token.content, path_env);
	if (!cmd_path)
	{
		ft_putstr_fd("Command not found:", STDERR_FILENO);
		ft_putendl_fd(cmd_node->cmd_token.content, STDERR_FILENO);
		res = 127;
	}
	else
	{
		argv = list_to_argv(cmd_node->arguments, cmd_path);
		if (!argv)
			res = EXIT_FAILURE;
		else
			res = custom_exec(cmd_path, argv, data->ep, fds);
		free_char_array(argv);
		free(cmd_path);
	}
	return (res);
}

int	execute_cmd_node(t_cmd_node *cmd_node, t_data *data)
{
	int	fds[2];
	int	res;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	if (expand(cmd_node, data))
		return (EXIT_FAILURE);
	if (set_redirections(&(cmd_node->redirections), fds))
		return (EXIT_FAILURE);
	res = EXIT_SUCCESS;
	if (cmd_node->cmd_token.type == BUILTIN)
		res = execute_builtin(cmd_node, data, fds);
	if (cmd_node->cmd_token.type == WORD)
		res = execute_cmd(cmd_node, data, fds);
	close_fds(fds);
	return (res);
}
