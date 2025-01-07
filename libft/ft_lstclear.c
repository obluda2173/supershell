/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:54:22 by erian             #+#    #+#             */
/*   Updated: 2024/12/29 14:11:28 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	head = *lst;
	if (!head)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		(*del)(tmp->content);
		free(tmp);
	}
	*lst = NULL;
}
