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

t_dllist	*ft_dllstmap(t_dllist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_dllist	*new_lst;
	t_dllist	*new_obj;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		new_obj = ft_dllstnew(f(lst->content));
		if (!new_obj)
		{
			ft_dllstclear(&new_lst, del);
			return (NULL);
		}
		ft_dllstadd_back(&new_lst, new_obj);
		lst = lst->next;
	}
	return (new_lst);
}
