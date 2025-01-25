#include "libft.h"
#include "minishell.h"
# include <readline/readline.h>
# include <readline/history.h>

//finding meeting line
char	*meeting_line(t_data **data)
{
	t_list	*start;
	char	*line;
	char	*result;

	if (!data || !*data)
		return (NULL);

	start = (*data)->ep;
	line = NULL;
	result = NULL;
	while (start)
	{
		t_env_var *env_var = (t_env_var*)start->content;
		if (ft_strcmp(env_var->key, "LOGNAME") == 0)
		{
			if (env_var->value) {
				line = ft_strdup(env_var->value);
			} else {
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
	return (NULL);
}

char * rl_gets(const char* prompt)
{
	static char *line_read = (char *)NULL;
	if (line_read)
	{
		free (line_read);
		line_read = (char *)NULL;
	}

	line_read = readline(prompt);

	if (line_read && *line_read)
		add_history (line_read);

	return (line_read);
}

char	*read_line_from_child(int read_fd)
{
	char	*buf;
	int		error;
	char	*line;

	line = NULL;
	line = malloc(sizeof(char) * 100);
	buf = line;
	error = read(read_fd, buf++, 1);
	if (error == 0)
	{
		free(line);
		close(read_fd);
		return (NULL);
	}
	while (read(read_fd, buf++, 1) > 0)
	{
	}
	buf = NULL;
	close(read_fd);
	return (line);
}
