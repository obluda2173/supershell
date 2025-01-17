/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/16 16:33:28 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
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
			argv[i] = NULL;
			char **temp_matrix = handle_wildcard(argument->word, argv);
			i = ft_matrix_size(temp_matrix) + 1;
			free_matrix(argv);
			argv = temp_matrix;
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

int execute_command(t_cmd_node cmd_node, char **ep, t_data *data)
{
	char *cmd_path;
	char **argv;

	int fds[2] = {STDIN_FILENO, STDOUT_FILENO};
	if (set_redirections(cmd_node.redirections, fds))
		return 1;

	int res = 0;
	if (cmd_node.cmd_token.type != NONE) {
		cmd_path = find_path(cmd_node.cmd_token.content, ep);
		if (!cmd_path)
		{
			fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
			return 127;
		}
		argv = list_to_argv(cmd_node.arguments, cmd_path, data);

		res = custom_exec(cmd_path, argv, ep, fds);
		free_matrix(argv);
		free(cmd_path);
	}
	close_fds(fds);

	return res;
}
