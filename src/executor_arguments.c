#include "executor.h"

void	expand_env(t_list *arguments, t_data *data)
{
	t_list	*head;
	char	*new_word;

	head = arguments;
	while (head)
	{
		if (((t_argument *)head->content)->type == DOUBLE_QUOTE_STR)
		{
			new_word = handle_double_quotes(((t_argument *)head->content)->word, data);
			free(((t_argument *)head->content)->word);
			((t_argument *)head->content)->word = new_word;
			((t_argument *)head->content)->type = LITERAL;
		}
		if (((t_argument *)head->content)->type == ENV_EXP
			|| ((t_argument *)head->content)->type == EXIT_STATUS_EXP)
		{
			new_word = handle_dollar(((t_argument *)head->content)->word, data);
			free(((t_argument *)head->content)->word);
			((t_argument *)head->content)->word = new_word;
			((t_argument *)head->content)->type = LITERAL;
		}
		head = head->next;
	}
}

int	expand_arguments(t_cmd_node *cmd_node, t_data *data)
{
	if (!cmd_node->arguments)
		return (EXIT_SUCCESS);
	if (expand_wildcards_in_arguments(&(cmd_node->arguments)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	expand_env(cmd_node->arguments, data);
	return (EXIT_SUCCESS);
}
