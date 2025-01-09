/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:36:00 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 17:08:46 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"

// int execute(t_script_node *script) {
// 	t_list* args = sn.node_data.cmd_node.arguments;
// 	while (args) {
// 		t_argument arg = *(t_argument*)args->content;
// 		ft_putstr_fd(arg.word, STDOUT_FILENO);
// 		args = args->next;
// 		if (args)
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 	}

// 	ft_putendl_fd("", STDOUT_FILENO);
// 	return 0;
// }

char **list_to_argv(t_list *list, char *cmd_path)
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
		t_argument *argument = (t_argument *)list->content;
		argv[i] = ft_strdup(argument->word);

		if (!argv[i++])
		{
			free_matrix(argv);
			return (NULL);
		}
		tmp = tmp->next;
	}
	argv[i] = NULL;

	return (argv);
}

int execute_command(t_cmd_node *cmd_node, char **envp)
{
	pid_t pid;
	int status;
	char *cmd_path;
	char **arg;

	cmd_path = find_path(cmd_node->cmd_token.content, envp);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd_node->cmd_token.content);
		return 127;
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return 1;
	}
	else if (pid == 0)
	{
		arg = list_to_argv(cmd_node->arguments, cmd_path);
		if (execve(cmd_path, arg, envp) == -1)
		{
			perror("execve");
			free(cmd_path);
			exit(1); // Exit with an error code
		}
	}
	else
	{
		free(cmd_path);
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return 1;
		}
		if (WIFEXITED(status))
		{
			return WEXITSTATUS(status);
		}
		else
		{
			fprintf(stderr, "Child process did not terminate normally\n");
			return 1;
		}
	}
	return 0;
}

int execute_script(t_script_node *script_node, char **envp)
{
	if (!script_node)
		return (0);


	if (script_node->node_type == CMD_NODE)
	{
		return execute_command(&(script_node->node_data.cmd_node), envp);
	}
	// else if (script_node->node_type == PIPE_NODE)
	// 	// Add logic for executing pipes
	// else if (script_node->node_type == AND_NODE || script_node->node_type == OR_NODE)
	// 	// Add logic for handling logical operators (&&, ||)
	// else if (script_node->node_type == ERROR_NODE)
	// {
	// 	fprintf(stderr, "minishell: syntax error: %s\n", script_node->node_data.error_node.error);
	// 	return (2); // Syntax error
	// }

	if (script_node->downstream)
		return execute_script(script_node->downstream, envp);

	return (0);
}
