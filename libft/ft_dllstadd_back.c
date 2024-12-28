/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dllstadd_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:28:32 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 18:00:37 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dllstadd_back(t_dllist **lst, t_dllist *new)
{
	t_dllist	*head;

	head = *lst;
	if (!head)
	{
		*lst = new;
		return ;
	}
	if (!new)
		return ;
	while (head->next)
	{
		head = head->next;
	}
	head->next = new;
	new->prev = head;
}
