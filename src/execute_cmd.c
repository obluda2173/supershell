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
#include "lexer.h"
#include "libft.h"
#include "parser.h"

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
	if (!argv[0])
	{
		free(argv);
		return (NULL);
	}

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
			char **temp_matrix = handle_wildcard(argument->word, argv);

			if (!temp_matrix)
			{
				free_matrix(argv);
				return (NULL);
			}
			free_matrix(argv);
			argv = temp_matrix;
			i = ft_matrix_size(argv);
			tmp = tmp->next;
			continue;
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
	}

	if (cmd_node.cmd_token.type == WORD)
	{
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
