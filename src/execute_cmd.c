/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 14:29:33 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int error_fork()
{
	perror("fork");
	return EXIT_FAILURE;
}


static int custom_exec(char *cmd_path, char **args, char **ep, int fds[2]) {
	pid_t pid = fork();
	int status;

	if (pid < 0)
		return error_fork();
	if (pid == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);

		if (execve(cmd_path, args, ep) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close_fds(fds);
		return EXIT_SUCCESS;
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status))
		return  WEXITSTATUS(status);
	fprintf(stderr, "Child process did not terminate normally\n");
	return EXIT_FAILURE;
}

int echo(t_cmd_node cmd_node, int fds[2], t_data *data) {
	char **argv = NULL;
	argv = list_to_argv(cmd_node.arguments, "", data);
	int res = 0;
	if (!argv)
		return 1;
	char** head = argv;
	head++;
	bool print_newline = true;
	if (!ft_strcmp(*head, "-n")) {
		print_newline = false;
		head++;
	}

	while (*head) {
		ft_putstr_fd(*head, fds[1]);
		head++;
		if (*head)
			ft_putstr_fd(" ", fds[1]);
	}
	if (print_newline)
		ft_putendl_fd("", fds[1]);

	free_matrix(argv);
	return res;
}

int export(char** ep) {
	char** head = ep;
	while (*head) {
		char** var = ft_split(*head, '=');
		if (var[1]) {
			printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		} else {
			printf("declare -x %s=\"(null)\"\n", var[0]);
		}
		free_matrix(var);
		head++;
	}
	return 0;
}

int execute_command(t_cmd_node cmd_node, char **ep, t_data *data)
{
	char *cmd_path;
	char **argv = NULL;

	int fds[2] = {STDIN_FILENO, STDOUT_FILENO};

	if (set_redirections(cmd_node.redirections, fds))
		return 1;

	int res = 0;
	if (cmd_node.cmd_token.type == BUILTIN) {
		if (!ft_strcmp("echo", cmd_node.cmd_token.content))
			res = echo(cmd_node, fds, data);
		if (!ft_strcmp("export", cmd_node.cmd_token.content)) {
			close_fds(fds);
			return export(ep);
		}
	}

	if (cmd_node.cmd_token.type == WORD)
	{
		if (!ft_strcmp(cmd_node.cmd_token.content, "which")) {
			if (!ft_strcmp(((t_argument*)cmd_node.arguments->content)->word, "echo")) {
				ft_putendl_fd("minishell built-in command", STDOUT_FILENO);
				close_fds(fds);
				return 0;
			}
		}
		cmd_path = find_path(cmd_node.cmd_token.content, ep);
		if (!cmd_path)
		{
			close_fds(fds);
			fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
			return 127;
		}
		argv = list_to_argv(cmd_node.arguments, cmd_path, data);
		if (!argv)
		{
			close_fds(fds);
			free(cmd_path);
			return 1;
		}
		res = custom_exec(cmd_path, argv, ep, fds);
		free_matrix(argv);
		free(cmd_path);
	}
	close_fds(fds);
	return res;
}
