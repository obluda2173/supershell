#include "executor.h"

int	expand_wildcards_in_arguments(t_list **list)
{
	t_list	*head;
	t_list	*new;
	t_list	dummy;

	if (!list || !*list)
		return (EXIT_FAILURE);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (head->next)
	{
		if (((t_argument *)head->next->content)->type == WILDCARD_EXP)
		{
			new = handle_wildcard_argument(*(t_argument *)head->next->content);
			if (!new)
				return (EXIT_FAILURE);
			sort_arguments(&new);
			replace_list_next_with_new(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}

void	expand_env(t_list *arguments, t_data *data)
{
	t_list		*head;
	char		*new_word;
	t_argument	*a;

	head = arguments;
	while (head)
	{
		a = (t_argument *)head->content;
		new_word = expand_variables(a->word, data);
		free(a->word);
		a->word = new_word;
		head = head->next;
	}
}

int	expand_arguments(t_cmd_node *cmd_node, t_data *data)
{
	if (!cmd_node->arguments)
		return (EXIT_SUCCESS);
	expand_env(cmd_node->arguments, data);
	if (expand_wildcards_in_arguments(&(cmd_node->arguments)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
