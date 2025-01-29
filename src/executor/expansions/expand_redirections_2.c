/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirections_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:15/53 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 18:15:53 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	is_current_dir(char *dir_path)
{
	return (!ft_strcmp(dir_path, "") || !ft_strcmp(dir_path, "."));
}

void	*teardown_redirect(char *dir_path, char *pattern, t_list *dir_entries)
{
	ft_putendl_fd("ambigious redirect", STDERR_FILENO);
	ft_lstclear(&dir_entries, free);
	free(dir_path);
	free(pattern);
	return (NULL);
}

t_list	*new_redirection_from_entry(char *entry, t_redirection redirection)
{
	t_redirection	*new;

	new = (t_redirection *)malloc(sizeof(t_redirection));
	new->word = ft_strdup(entry);
	new->word_type = LITERAL;
	new->type = redirection.type;
	new->fd = redirection.fd;
	return (ft_lstnew(new));
}

int	teardown_close_fds(int fds[2], char *err_msg)
{
	perror(err_msg);
	close_fds(fds);
	return (EXIT_FAILURE);
}
