/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 19:25:44 by erian             #+#    #+#             */
/*   Updated: 2024/08/07 20:16:08 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst_ret;
	t_list	*current;

	if (!lst || !f)
		return (NULL);
	lst_ret = ft_lstnew(f(lst->content));
	if (!lst_ret)
		return (NULL);
	current = lst_ret;
	while (lst->next)
	{
		lst = lst->next;
		current->next = ft_lstnew(f(lst->content));
		if (!current->next)
		{
			ft_lstclear(&lst_ret, del);
			return (NULL);
		}
		current = current->next;
	}
	return (lst_ret);
}
