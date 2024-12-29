/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dllstclear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:54:22 by erian             #+#    #+#             */
/*   Updated: 2024/12/29 14:11:59 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dllstclear(t_dllist **dllst, void (*del)(void *))
{
	t_dllist	*head;
	t_dllist	*tmp;

	head = *dllst;
	if (!head)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		(*del)(tmp->content);
		free(tmp);
	}
	*dllst = NULL;
}
