/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 08:44/57 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 08:44:57 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 1024

#include "minishell.h"

void	*teardown_content(char *content, char *err_msg)
{
	free(content);
	ft_putendl_fd(err_msg, STDERR_FILENO);
	return (NULL);
}

char	*meeting_line(t_data **data)
{
	t_list		*start;
	char		*line;
	char		*result;
	t_env_var	*env_var;

	if (!data || !*data)
		return (NULL);
	start = (*data)->ep;
	line = NULL;
	result = NULL;
	while (start)
	{
		env_var = (t_env_var *)start->content;
		if (ft_strcmp(env_var->key, "LOGNAME") == 0)
		{
			if (env_var->value)
				line = ft_strdup(env_var->value);
			else
				line = ft_strdup("");
			if (line)
			{
				result = ft_strjoin(line, "$ ");
				free(line);
				return (result);
			}
		}
		start = start->next;
	}
	return (ft_strdup("$ "));
}

char	*rl_gets(const char *prompt)
{
	static char	*line_read = (char *)NULL;

	if (line_read)
	{
		free(line_read);
		line_read = (char *)NULL;
	}
	line_read = readline(prompt);
	if (line_read && *line_read)
		add_history(line_read);
	return (line_read);
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

char	*read_line_from_fd(int fd)
{
	char	buffer[BUFFER_SIZE];
	char	*content;
	size_t	total_size;
	ssize_t	bytes_read;
	char	*new_content;

	content = NULL;
	total_size = 0;
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		new_content = my_realloc(content, total_size + bytes_read + 1);
		if (!new_content)
			return (teardown_content(content, "Failed to allocate memory"));
		content = new_content;
		memcpy(content + total_size, buffer, bytes_read);
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
