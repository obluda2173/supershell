/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:17:23 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 17:10:09 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>

char	**ep_to_char_array(int fds[2], t_list *ep)
{
	int		count;
	char	**env_matrix;
	char	*first;
	char	*second;

	env_matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(ep) + 1));
	if (!env_matrix)
		teardown_close_fds(fds, "Allocation error");
	count = 0;
	while (ep)
	{
		first = ft_strjoin(((t_env_var *)ep->content)->key, "=");
		if (((t_env_var *)ep->content)->value)
			second = ft_strjoin(first, ((t_env_var *)ep->content)->value);
		else
			second = ft_strdup(first);
		env_matrix[count++] = ft_strdup(second);
		free(first);
		free(second);
		ep = ep->next;
	}
	env_matrix[count] = NULL;
	return (env_matrix);
}

void	child_exec_cmd(int fds[2], t_list *ep, char *cmd_path, char **args)
{
	char	**env_matrix;

	signal(SIGQUIT, SIG_DFL);
	env_matrix = ep_to_char_array(fds, ep);
	if (!env_matrix)
		exit(EXIT_FAILURE);
	if (dup2(fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	};
	close_fds(fds);
	if (execve(cmd_path, args, env_matrix) == -1)
	{
		free_char_array(env_matrix);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	free_char_array(env_matrix);
	exit(EXIT_SUCCESS);
}

int	custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
		child_exec_cmd(fds, ep, cmd_path, args);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (status == 131)
		ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
	else
		ft_putendl_fd("", STDOUT_FILENO);
	return (status);
}
