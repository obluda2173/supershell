/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:25:44 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 11:25:33 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_ep	*ft_lstmap(t_ep *lst, void *(*f)(void *), void (*del)(void *))
{
	t_ep	*lst_ret;
	t_ep	*current;

	if (!lst || !f)
		return (NULL);
	lst_ret = ft_lstnew(f(lst->value));
	if (!lst_ret)
		return (NULL);
	current = lst_ret;
	while (lst->next)
	{
		lst = lst->next;
		current->next = ft_lstnew(f(lst->value));
		if (!current->next)
		{
			ft_lstclear(&lst_ret, del);
			return (NULL);
		}
		current = current->next;
	}
	return (lst_ret);
}
