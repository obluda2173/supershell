#include "executor.h"

int	ft_strcmp2(const char *s1, const char *s2)
{
	char	*s1_copy;
	char	*s2_copy;
	char	*ptr;
	int		result;

	s1_copy = ft_strdup(s1);
	s2_copy = ft_strdup(s2);
	ptr = s1_copy;
	while (*ptr)
	{
		*ptr = ft_tolower(*ptr);
		ptr++;
	}
	ptr = s2_copy;
	while (*ptr)
	{
		*ptr = ft_tolower(*ptr);
		ptr++;
	}
	result = ft_strcmp(s1_copy, s2_copy);
	free(s1_copy);
	free(s2_copy);
	return (result);
}

void	sort_arguments(t_list **list)
{
	t_list	*head;
	int		len;
	t_list	dummy;
	t_list	*prev;
	t_list	*curr;
	int		count;

	if (!list || !*list)
		return ;
	len = ft_lstsize(*list);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (len > 1)
	{
		prev = head;
		curr = prev->next;
		count = 0;
		while (count < len - 1)
		{
			count++;
			if (ft_strcmp2(((t_argument *)curr->content)->word,
					((t_argument *)curr->next->content)->word) < 0)
			{
				prev = curr;
				curr = curr->next;
			}
			else
			{
				prev->next = curr->next;
				curr->next = curr->next->next;
				prev->next->next = curr;
				curr = prev->next->next;
				prev = prev->next;
			}
		}
		len--;
	}
	*list = dummy.next;
}
