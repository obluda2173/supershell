#include "executor.h"

void	expand_env(t_list *arguments, t_data *data)
{
	t_list		*head;
	char		*new_word;
	t_argument	*a;

	head = arguments;
	while (head)
	{
		a = (t_argument *)head->content;
		if (a->type == DOUBLE_QUOTE_STR)
		{
			new_word = handle_double_quotes(a->word, data);
			free(a->word);
			a->word = new_word;
			a->type = LITERAL;
		}
		if (a->type == ENV_EXP || a->type == EXIT_STATUS_EXP)
		{
			new_word = handle_dollar(a->word, data);
			free(a->word);
			a->word = new_word;
			a->type = LITERAL;
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
