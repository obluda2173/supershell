/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 07:06/41 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 07:06:41 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	exec_pipe_downstream(int input_fd, t_script_node *sn, t_data *data)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
		exit(execute_script(sn->downstream, data));
	}
	close(input_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

void	exec_upstream_child(int pipedes[2], t_script_node *sn, t_data *data)
{
	dup2(pipedes[1], STDOUT_FILENO);
	close(pipedes[0]);
	close(pipedes[1]);
	exit(execute_script(sn->upstream, data));
}

int	execute_pipeline(t_script_node *sn, t_data *data)
{
	int		pipedes[2];
	int		status;
	pid_t	pid;

	if (pipe(pipedes) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
		exec_upstream_child(pipedes, sn, data);
	close(pipedes[1]);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		return (exec_pipe_downstream(pipedes[0], sn, data));
	return (EXIT_FAILURE);
}
