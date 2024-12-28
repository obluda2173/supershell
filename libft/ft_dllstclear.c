/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:54:22 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 11:24:40 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dllstclear(t_dllist **lst, void (*del)(void *))
{
	t_dllist	*head;
	t_dllist	*tmp;

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
