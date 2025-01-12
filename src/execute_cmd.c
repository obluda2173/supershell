/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 12:57:32 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static char **list_to_argv(t_list *list, char *cmd_path, t_data *data)
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
			processed_word = handle_double_quotes(argument->word, data);
		else if (argument->type == EXIT_STATUS_EXP || argument->type == ENV_EXP)
			processed_word = handle_dollar(argument->word, data);
		else if (argument->type == WILDCARD_EXP)
		{
			processed_word = handle_wildcard(argument->word);
			printf("line: %s\n", processed_word);
		}
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
	return EXIT_FAILURE;
}

static int custom_exec(char *cmd_path, char **args, char **ep, int fd_in) {
	pid_t pid = fork();
	int status;

	if (pid < 0)
		return error_fork();
	if (pid == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		if (execve(cmd_path, args, ep) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close(fd_in);
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

int execute_command(t_cmd_node cmd_node, char **ep, t_data *data)
{
	char *cmd_path;
	char **argv;

	if (cmd_node.cmd_token.type == NONE)
		return 0;
	cmd_path = find_path(cmd_node.cmd_token.content, ep);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
		return 127;
	}
	argv = list_to_argv(cmd_node.arguments, cmd_path, data);

	int fd = 0;
	if (cmd_node.redirections) {
		t_list *head = cmd_node.redirections;
		fd = open(((t_redirection*)head->content)->word, O_RDONLY);
		if (fd < 0) {
			perror(((t_redirection*)head->content)->word);
			close(fd);
			free_matrix(argv);
			free(cmd_path);
			return 1;
		}
	}

	int res = custom_exec(cmd_path, argv, ep, fd);
	if (fd) {
		close(fd);
	}

	free_matrix(argv);
	free(cmd_path);

	return res;
}
