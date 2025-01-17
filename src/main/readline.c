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
		if (ft_strncmp(start->content, "LOGNAME=", 8) == 0)
		{
			line = ft_substr(start->content, 8, ft_strlen(start->content) - 8);
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
