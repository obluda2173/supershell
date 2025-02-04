/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:17:23 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 17:10:09 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <unistd.h>

char	**ep_to_char_array(int fds[2], t_list *ep)
{
	int		count;
	char	**env_matrix;
	char	*first;
	char	*second;

	env_matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(ep) + 1));
	if (!env_matrix)
		teardown_close_fds(fds, "Allocation error");
	count = 0;
	while (ep)
	{
		first = ft_strjoin(((t_env_var *)ep->content)->key, "=");
		if (((t_env_var *)ep->content)->value)
			second = ft_strjoin(first, ((t_env_var *)ep->content)->value);
		else
			second = ft_strdup(first);
		env_matrix[count++] = ft_strdup(second);
		free(first);
		free(second);
		ep = ep->next;
	}
	env_matrix[count] = NULL;
	return (env_matrix);
}

void	child_exec_cmd(int fds[2], t_list *ep, char *cmd_path, char **args)
{
	char	**env_matrix;
	int		exit_status;

	signal(SIGQUIT, SIG_DFL);
	env_matrix = ep_to_char_array(fds, ep);
	if (!env_matrix)
		exit(EXIT_FAILURE);
	if (dup2(fds[0], STDIN_FILENO) == -1)
		exit_perror("dup2", EXIT_FAILURE);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		exit_perror("dup2", EXIT_FAILURE);
	close_fds(fds);
	if (execve(cmd_path, args, env_matrix) == -1)
	{
		exit_status = errno;
		perror("execve");
		free_char_array(env_matrix);
		if (exit_status == EACCES)
			exit(126);

		exit(exit_status);
	}
	free_char_array(env_matrix);
	exit(EXIT_SUCCESS);
}

int	custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
		child_exec_cmd(fds, ep, cmd_path, args);
	if (waitpid(-1, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status)) {
		g_signal_received = 0;
		return (WEXITSTATUS(status));
	}
	if (status == 131)
		ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
	else
		ft_putendl_fd("", STDOUT_FILENO);
	return (status);
}
