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

static int	custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2])
{
	pid_t	pid;
	char	**env_matrix;
	int		status;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		env_matrix = ep_to_matrix(fds, ep);
		if (!env_matrix)
			exit(EXIT_FAILURE);
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		if (execve(cmd_path, args, env_matrix) == -1)
		{
			free_char_array(env_matrix);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close_fds(fds);
		free_char_array(env_matrix);
		return (EXIT_SUCCESS);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (status == 131)
		ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
	else
		ft_putendl_fd("", STDOUT_FILENO);
	return (status);
}

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
		fprintf(stderr, "Command not found: %s\n", cmd_node->cmd_token.content);
		res = 127;
	}
	else
	{
		argv = list_to_argv(cmd_node->arguments, cmd_path);
		if (!argv)
			res = EXIT_FAILURE;
		else
		{
			res = custom_exec(cmd_path, argv, data->ep, fds);
			free_char_array(argv);
		}
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
