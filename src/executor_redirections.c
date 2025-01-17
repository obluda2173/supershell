/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:40/12 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/17 12:40:12 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

void	close_fds(int fds[2])
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return ;
}

int	set_output(t_redirection r, int fds[2], int hered_pipe[2])
{
	(void)hered_pipe;
	if (r.type == OUT)
	{
		fds[1] = open(r.word, O_CREAT | O_WRONLY | O_TRUNC,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	if (r.type == APPEND)
	{
		fds[1] = open(r.word, O_APPEND | O_WRONLY | O_CREAT,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int	set_input(t_redirection r, int fds[2], int hered_pipe[2])
{
	if (r.type == IN)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = open(r.word, O_RDONLY, NULL);
		if (fds[0] < 0)
		{
			perror("open");
			close_fds(fds);
			return (EXIT_FAILURE);
		}
	}
	if (r.type == HERED)
	{
		pipe(hered_pipe);
		write(hered_pipe[1], r.word, ft_strlen(r.word));
		close(hered_pipe[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = hered_pipe[0];
	}
	return (EXIT_SUCCESS);
}


int	set_redirections(t_list *redirections, int fds[2])
{
	t_list			*head;
	t_redirection	r;
	int				hered_pipe[2];

	head = redirections;
	while (head)
	{
		r = *((t_redirection *)head->content);
		if (set_input(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (set_output(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		head = head->next;
	}
	return (0);
}
