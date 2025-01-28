#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

// finding meeting line
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
			{
				line = ft_strdup(env_var->value);
			}
			else
			{
				line = ft_strdup("");
			}
			if (line)
			{
				result = ft_strjoin(line, "$ ");
				free(line);
				return (result);
			}
		}
		start = start->next;
	}
	return ft_strdup("$ ");
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

/* char	*read_line_from_child(int read_fd) */
/* { */
/* 	int		error; */
/* 	char	*line; */
/* 	int		count; */

/* 	line = NULL; */
/* 	line = malloc(sizeof(char) * 100); */
/* 	count = 0; */
/* 	error = read(read_fd, line + (count++), 1); */
/* 	if (error == 0) */
/* 	{ */
/* 		free(line); */
/* 		close(read_fd); */
/* 		return (NULL); */
/* 	} */
/* 	while (read(read_fd, line + (count++), 1) > 0) */
/* 	{ */
/* 	} */
/* 	close(read_fd); */
/* 	return (line); */
/* } */

#define BUFFER_SIZE 1024

void *my_realloc(void *ptr, size_t new_size) {
	if (new_size == 0) {
		free(ptr);
		return NULL;
	}

	if (ptr == NULL)
		return malloc(new_size);

	void *new_ptr = malloc(new_size);
	if (!new_ptr)
		return NULL;  // Allocation failed

	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return new_ptr;
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
		{
			free(content);
			perror("Failed to allocate memory");
			return (NULL);
		}
		content = new_content;
		memcpy(content + total_size, buffer, bytes_read);
		total_size += bytes_read;
	}
	if (bytes_read == -1)
	{
		perror("Error reading from file descriptor");
		free(content);
		return (NULL);
	}
	if (bytes_read && !content)
		return (NULL);
	if (content)
		content[total_size] = '\0'; // Null-terminate the string
	close(fd);
	return (content);
}
