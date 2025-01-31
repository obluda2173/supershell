/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 16:21:14 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_builtins.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>
#include <sys/stat.h>

int	expand(t_cmd_node *cn, t_data *data)
{
	char	*new_word;

	if (cn->cmd_token.content)
	{
		new_word = expand_string(cn->cmd_token.content, data);
		if (!new_word)
		{
			ft_putendl_fd("Syntax Error: could not expand command token",
				STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		free(cn->cmd_token.content);
		cn->cmd_token.content = new_word;
	}
	if (expand_redirections(cn, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (expand_arguments(cn, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (cn->cmd_token.content && ft_strlen(cn->cmd_token.content) == 0) {
		t_list* next_arg = NULL;
		t_list* head = cn->arguments;
		while (head) {
			if (ft_strlen(((t_argument*)head->content)->word) == 0) {
				head = head->next;
				continue;
			}
			next_arg = head;
			head = head->next;
			free(cn->cmd_token.content);

			cn->cmd_token.content = ft_strdup(((t_argument*)next_arg->content)->word);
			next_arg->next = NULL;
			ft_lstclear(&cn->arguments, free_arguments);
			cn->arguments = head;
			break;
		}
	}
	return (EXIT_SUCCESS);
}

int	execute_builtin(t_cmd_node *cmd_node, t_data *data, int fds[2])
{
	if (!ft_strcmp("echo", cmd_node->cmd_token.content))
		return (echo(*cmd_node, fds));
	if (!ft_strcmp("export", cmd_node->cmd_token.content))
		return (cstm_export(&data->ep, cmd_node));
	if (!ft_strcmp("unset", cmd_node->cmd_token.content))
		return (cstm_unset(&data->ep, cmd_node));
	if (!ft_strcmp("env", cmd_node->cmd_token.content))
		return (cstm_env(&data->ep, cmd_node));
	if (!ft_strcmp("pwd", cmd_node->cmd_token.content))
		return (cstm_pwd(&data->ep, cmd_node));
	if (!ft_strcmp("cd", cmd_node->cmd_token.content))
		return (cstm_cd(&data->ep, cmd_node->arguments));
	if (!ft_strcmp("exit", cmd_node->cmd_token.content))
		return (cstm_exit(cmd_node->arguments, data));
	return (EXIT_SUCCESS);
}

bool is_builtin(char *word) {
	if (!word)
		return false;
	if (!ft_strcmp("echo", word))
		return true;
	if (!ft_strcmp("export", word))
		return true;
	if (!ft_strcmp("unset", word))
		return true;
	if (!ft_strcmp("env", word))
		return true;
	if (!ft_strcmp("pwd", word))
		return true;
	if (!ft_strcmp("cd", word))
		return true;
	if (!ft_strcmp("exit", word))
		return true;
	return false;

}

int is_directory(const char *path) {
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		perror("stat");
		return 0;
	}
	return S_ISDIR(path_stat.st_mode);
}

int	execute_cmd(t_cmd_node *cmd_node, t_data *data, int fds[2])
{
	char	*path_env;
	char	*cmd_path;
	int		res;
	char	**argv;

	argv = NULL;
	path_env = get_path_env(data->ep);
	cmd_path = find_path(cmd_node->cmd_token.content, path_env);
	if (!cmd_path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd_node->cmd_token.content, STDERR_FILENO);
		res = 127;
	} else if (is_directory(cmd_path)) {
		ft_putendl_fd("Is a directory", STDERR_FILENO);
		free(cmd_path);
		res = 126;
	} else {
		argv = list_to_argv(cmd_node->arguments, cmd_path);
		if (!argv)
			res = EXIT_FAILURE;
		else
			res = custom_exec(cmd_path, argv, data->ep, fds);
		free_char_array(argv);
		free(cmd_path);
	}
	return (res);
}

int	execute_cmd_node(t_cmd_node *cn, t_data *data)
{
	int	fds[2];
	int	res;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	if (expand(cn, data))
		return (EXIT_FAILURE);
	if (set_redirections(&(cn->redirections), fds))
		return (EXIT_FAILURE);
	res = EXIT_SUCCESS;
	if (is_builtin(cn->cmd_token.content))
		res = execute_builtin(cn, data, fds);
	else if (cn->cmd_token.type == WORD && ft_strlen(cn->cmd_token.content))
		res = execute_cmd(cn, data, fds);
	close_fds(fds);
	return (res);
}
