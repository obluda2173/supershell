/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/10 16:09:16 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "mock_system_calls.h"

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

	argv[0] = cmd_path;

	tmp = list;
	while (tmp)
	{
		t_argument *argument = (t_argument *)tmp->content;
		char *processed_word = NULL;

		if (argument->type == LITERAL)
			processed_word = ft_strdup(argument->word);
		else if (argument->type == DOUBLE_QUOTE_STR)
			processed_word = handle_double_quotes(argument->word);
		else if (argument->type == ENV_EXP)
			processed_word = getenv(argument->word);
		// else if (argument->type == WILDCARD_EXP)
		// 	processed_word = handle_wildcard_expansion(argument->word);
		// else if (argument->type == EXIT_STATUS_EXP)
		// 	processed_word = ft_itoa(get_exit_status());

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

static int custom_exec(char *cmd_path, char **args, char **envp, t_system_calls sc) {
	pid_t pid = sc.fork();
	int status;

	if (pid < 0)
	{
		perror("fork");
		return 1;
	}
	if (pid == 0)
	{
		if (sc.execve(cmd_path, args, envp) == -1)
		{
			perror("execve");
			exit(1); // Exit with an error code
		}
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	fprintf(stderr, "Child process did not terminate normally\n");
	return 1;
}

int execute_command(t_cmd_node cmd_node, char **envp, t_system_calls sc)
{
	char *cmd_path;
	char **args;

	cmd_path = find_path(cmd_node.cmd_token.content, envp);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
		return 127;
	}
	args = list_to_argv(cmd_node.arguments, cmd_path);
	int res = custom_exec(cmd_path, args, envp, sc);
	free_matrix(args);

	return res;
}
