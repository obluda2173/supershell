/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_arguments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:35:35 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/31 13:12:14 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	swap_nodes(t_list *prev, t_list *curr)
{
	prev->next = curr->next;
	curr->next = curr->next->next;
	prev->next->next = curr;
}

void	bubble_sort_inner_loop(t_list *head, int len)
{
	t_list	*prev;
	t_list	*curr;
	int		count;

	prev = head;
	curr = prev->next;
	count = 0;
	while (count++ < len - 1)
	{
		if (ft_strcmp(((t_argument *)curr->content)->word,
				((t_argument *)curr->next->content)->word) < 0)
		{
			prev = curr;
			curr = curr->next;
		}
		else
		{
			swap_nodes(prev, curr);
			curr = prev->next->next;
			prev = prev->next;
		}
	}
}

void	sort_arguments(t_list **list)
{
	t_list	*head;
	int		len;
	t_list	dummy;

	if (!list || !*list)
		return ;
	len = ft_lstsize(*list);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (len > 1)
	{
		bubble_sort_inner_loop(head, len);
		len--;
	}
	*list = dummy.next;
}
