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
#include "parser.h"

void	replace_command_token_with_first_arg(t_cmd_node *cn)
{
	t_list	*n_arg;
	t_list	*head;
	char	*word;

	n_arg = NULL;
	head = cn->arguments;
	while (head)
	{
		if (ft_strlen(((t_argument *)head->content)->word) == 0)
		{
			head = head->next;
			continue ;
		}
		n_arg = head;
		head = head->next;
		free(cn->cmd_token.content);
		word = ft_strdup(((t_argument *)n_arg->content)->word);
		cn->cmd_token.content = word;
		n_arg->next = NULL;
		ft_lstclear(&cn->arguments, free_arguments);
		cn->arguments = head;
		break ;
	}
}

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

	if (!cn->cmd_token.content) {
		replace_command_token_with_first_arg(cn);
		cn->cmd_token.type = WORD;
	}
	if (cn->cmd_token.content && ft_strlen(cn->cmd_token.content) == 0)
		replace_command_token_with_first_arg(cn);
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
		return (error_cmd_not_found(cmd_node));
	else if (is_directory(cmd_path))
		return (error_is_directory(cmd_path));
	else
	{
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
