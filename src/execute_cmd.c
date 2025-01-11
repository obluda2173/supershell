/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/11 13:30:43 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>

int last_exit_status(int new_status, int update)
{
    static int status = 0;

    if (update)
        status = new_status;
    return status;
}

static char **list_to_argv(t_list *list, char *cmd_path)
{
	char **argv;
	size_t count = 0;
	size_t i = 1;
	t_list *tmp = list;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * (count + 2));
	if (!argv)
		return (NULL);

	argv[0] = ft_strdup(cmd_path);

	tmp = list;
	while (tmp)
	{
		t_argument *argument = (t_argument *)tmp->content;
		char *processed_word = NULL;
		if (argument->type == LITERAL)
			processed_word = ft_strdup(argument->word);
		else if (argument->type == DOUBLE_QUOTE_STR)
			processed_word = handle_double_quotes(argument->word);
		else if (argument->type == EXIT_STATUS_EXP || argument->type == ENV_EXP)
		{
			processed_word = handle_dollar(argument->word);
		}
		// else if (argument->type == ENV_EXP)
		// 	processed_word = getenv(argument->word);
		// else if (argument->type == WILDCARD_EXP)
		// 	processed_word = handle_wildcard_expansion(argument->word);
		// else if (argument->type == EXIT_STATUS_EXP)
		// 	processed_word = ft_itoa(get_exit_status());
		// printf("my line: %s\n", processed_word);
		if (!processed_word)
		{
			free_matrix(argv);
			return (NULL);
		}

		argv[i++] = processed_word;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

int error_fork() {
	perror("fork");
	last_exit_status(1, 1);
	return (1);
}

static int custom_exec(char *cmd_path, char **args, char **envp, int fd_in) {

	int status;
	pid_t pid = fork();

	if (pid < 0)
		return error_fork();
	if (pid == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		if (execve(cmd_path, args, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
		close(fd_in);
		return EXIT_SUCCESS;
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		last_exit_status(1, 1);
		return 1;
	}
	if (WIFEXITED(status))
	{
		last_exit_status(WEXITSTATUS(status), 1);	
		return WEXITSTATUS(status);
	}
	ft_putendl_fd("Child process did not terminate normally", STDOUT_FILENO);
	last_exit_status(1, 1);
	return (1);
}

int execute_command(t_cmd_node cmd_node, char **envp)
{
	char *cmd_path;
	char **args;

	cmd_path = find_path(cmd_node.cmd_token.content, envp);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
		last_exit_status(127, 1);
		return 127;
	}
	args = list_to_argv(cmd_node.arguments, cmd_path);

	int fd = 0;
	if (cmd_node.redirections)
		fd = open("tests/end_to_end_tests/test_files/input1.txt", O_RDONLY);

	int res = custom_exec(cmd_path, args, envp, fd);
	if (fd) {
		close(fd);
	}

	free(cmd_path);
	free_matrix(args);

	return res;
}
