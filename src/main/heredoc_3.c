/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:21:33 by erian             #+#    #+#             */
/*   Updated: 2025/01/31 17:31:06 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted_delimiter(char *delimiter)
{
	if (ft_strchr(delimiter, '\''))
		return (true);
	if (ft_strchr(delimiter, '\"'))
		return (true);
	return (false);
}

int	process_heredoc_delimiter(t_dllist **heredoc_token, t_data *data,
	char **delimiter, bool *quoted)
{
	char	*new_delim;

	*delimiter = extract_delimiter(heredoc_token);
	if (!*delimiter)
	{
		data->exit_status = 2;
		return (EXIT_FAILURE);
	}
	*quoted = is_quoted_delimiter(*delimiter);
	if (*quoted)
	{
		new_delim = remove_quotes(*delimiter);
		free(*delimiter);
		*delimiter = new_delim;
	}
	return (EXIT_SUCCESS);
}

int	replace_token(t_dllist *heredoc_token, t_dllist *new_node, t_data *data)
{
	if (heredoc_token->next)
	{
		new_node->next = heredoc_token->next;
		new_node->next->prev = new_node;
	}
	else
		new_node->next = NULL;
	new_node->prev = heredoc_token->prev;
	heredoc_token->prev->next = new_node;
	ft_dllstdelone(heredoc_token, free_token);
	return (heredoc_loop(&new_node->next, data));
}
