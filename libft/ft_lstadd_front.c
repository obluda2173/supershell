/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:57:06 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 11:24:35 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_ep **lst, t_ep *new_node)
{
	if (!lst || !new_node)
		return ;
	new_node->next = *lst;
	*lst = new_node;
}
