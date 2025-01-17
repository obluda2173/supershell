#include "minishell.h"

//initialise data structure and extract environment
void	init(t_data **data, char **ep)
{
	t_list	*new_node;
	t_list	*last_node;

	//initialise all data needed
	*data = malloc(sizeof(t_data));
	if (!*data)
		return ;
	(*data)->ep = NULL;
	(*data)->exit_status = 0;
	(*data)->not_exit = true;

	//extract environment
	while (*ep != NULL)
	{
		new_node = ft_lstnew(*ep);
		if (!new_node)
		{
			free_all(data);
			return ;
		}
		last_node = ft_lstlast((*data)->ep);
		if (!last_node)
			(*data)->ep = new_node;
		else
			ft_lstadd_back(&((*data)->ep), new_node);
		ep++;
	}
}
