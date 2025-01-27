/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:36:00 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 10:53:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <sys/wait.h>
#include <unistd.h>

int	execute_script(t_script_node *script_node, t_data *data)
{
	if (!script_node)
		return (0);
	if (script_node->node_type == CMD_NODE)
		return execute_command(&script_node->node_data.cmd_node, data);
	if (script_node->node_type == PIPE_NODE) {
		int pipedes[2];
		int status;
		if (pipe(pipedes) == -1)
		{
			perror("pipe");
			return EXIT_FAILURE;
		}
		pid_t pid = fork();
		if (pid < 0)
			return (error_fork());
		if (pid == 0) {
			dup2(pipedes[1], STDOUT_FILENO);
			close(pipedes[0]);
			close(pipedes[1]);
			execute_script(script_node->upstream, data);
			exit(EXIT_SUCCESS);
		}
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return EXIT_FAILURE;
		}
		if (WIFEXITED(status)) {
			close(pipedes[1]);

			pid_t pid = fork();
			if (pid < 0)
				return (error_fork());
			if (pid == 0) {
				dup2(pipedes[0], STDIN_FILENO);
				execute_script(script_node->downstream, data);
				exit(EXIT_SUCCESS);
			} else {
				close(pipedes[0]);
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					return (WEXITSTATUS(status));
				return EXIT_FAILURE;
			}
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
