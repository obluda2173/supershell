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
#include "libft.h"
#include "parser.h"
#include <unistd.h>

int	error_fork(void)
{
	perror("fork");
	return (EXIT_FAILURE);
}

static int	custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2])
{
	pid_t	pid;
	char	**env_matrix;
	int		status;
	int		count;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
	{
		env_matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(ep) + 1));
		if (!env_matrix)
		{
			close_fds(fds);
			ft_putendl_fd("Allocation error", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		count = 0;
		while (ep)
		{
			env_matrix[count] = ft_strdup((char *)ep->content);
			ep = ep->next;
			count++;
		}
		env_matrix[count] = NULL;
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		if (execve(cmd_path, args, env_matrix) == -1)
		{
			free_matrix(env_matrix);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close_fds(fds);
		free_matrix(env_matrix);
		return (EXIT_SUCCESS);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	fprintf(stderr, "Child process did not terminate normally\n");
	return (EXIT_FAILURE);
}

int	echo(t_cmd_node cmd_node, int fds[2])
{
	char	**argv;
	int		res;
	char	**head;
	bool	print_newline;

	argv = NULL;
	argv = list_to_argv(cmd_node.arguments, "");
	res = 0;
	if (!argv)
		return (1);
	head = argv;
	head++;
	if (*head)
	{
		print_newline = true;
		if (!ft_strcmp(*head, "-n"))
		{
			print_newline = false;
			head++;
		}
		while (*head)
		{
			ft_putstr_fd(*head, fds[1]);
			head++;
			if (*head)
				ft_putstr_fd(" ", fds[1]);
		}
		if (print_newline)
			ft_putendl_fd("", fds[1]);
	}
	else
	{
		ft_putendl_fd("", fds[1]);
	}
	free_matrix(argv);
	return (res);
}

int	export(t_list *ep)
{
	char	**var;

	while (ep)
	{
		var = ft_split((char *)ep->content, '=');
		if (var[1])
		{
			printf("declare -x %s=\"%s\"\n", var[0], var[1]);
		}
		else
		{
			printf("declare -x %s=\"(null)\"\n", var[0]);
		}
		free_matrix(var);
		ep = ep->next;
	}
	return (0);
}

int	execute_command(t_cmd_node *cmd_node, t_data *data)
{
	char	*cmd_path;
	char	**argv;
	int		fds[2] = {STDIN_FILENO, STDOUT_FILENO};
	int		res;
	char	*path_env;

	argv = NULL;


	if (expand_redirections(cmd_node, data) == EXIT_FAILURE)
	{
		return (EXIT_FAILURE);
	}
	if (expand_arguments(cmd_node, data) == EXIT_FAILURE)
	{
		return (EXIT_FAILURE);
	}

	if (set_redirections(&(cmd_node->redirections), fds))
		return (EXIT_FAILURE);
	res = 0;
	if (cmd_node->cmd_token.type == BUILTIN)
	{
		if (!ft_strcmp("echo", cmd_node->cmd_token.content))
			res = echo(*cmd_node, fds);
		if (!ft_strcmp("export", cmd_node->cmd_token.content))
		{
			close_fds(fds);
			return (export(data->ep));
		}
	}
	if (cmd_node->cmd_token.type == WORD)
	{
		if (!ft_strcmp(cmd_node->cmd_token.content, "which"))
		{
			if (!ft_strcmp(((t_argument *)cmd_node->arguments->content)->word,
					"echo"))
			{
				ft_putendl_fd("minishell built-in command", STDOUT_FILENO);
				close_fds(fds);
				return (0);
			}
		}
		path_env = get_path_env(data->ep);
		cmd_path = find_path(cmd_node->cmd_token.content, path_env);
		if (!cmd_path)
		{
			close_fds(fds);
			fprintf(stderr, "Command not found: %s\n",
				cmd_node->cmd_token.content);
			return (127);
		}
		argv = list_to_argv(cmd_node->arguments, cmd_path);
		if (!argv)
		{
			close_fds(fds);
			free(cmd_path);
			return (1);
		}
		res = custom_exec(cmd_path, argv, data->ep, fds);
		free_matrix(argv);
		free(cmd_path);
	}
	close_fds(fds);
	return (res);
}
