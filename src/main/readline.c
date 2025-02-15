/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 08:44:57 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 09:55:50 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#define BUFFER_SIZE 1024

#include "minishell.h"

void	*teardown_content(char *content, char *err_msg)
{
	free(content);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	return (NULL);
}

void	*my_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

char	*set_content(size_t total_size, ssize_t bytes_read, char *content,
		char *buffer)
{
	char	*new_content;

	new_content = my_realloc(content, total_size + bytes_read + 1);
	if (!new_content)
		return (teardown_content(content, "Failed to allocate memory"));
	content = new_content;
	ft_memcpy(content + total_size, buffer, bytes_read);
	return (new_content);
}

char	*read_line_from_fd(int fd)
{
	char	buffer[BUFFER_SIZE];
	char	*content;
	size_t	total_size;
	ssize_t	bytes_read;

	content = NULL;
	total_size = 0;
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		content = set_content(total_size, bytes_read, content, buffer);
		total_size += bytes_read;
	}
	if (bytes_read == -1)
		return (teardown_content(content,
				"Error reading from file descriptor"));
	if (bytes_read && !content)
		return (NULL);
	if (content)
		content[total_size] = '\0';
	close(fd);
	return (content);
}
