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

void	ft_dllstadd_front(t_dllist **lst, t_dllist *new)
{
	t_dllist	*tmp;

	tmp = *lst;
	*lst = new;
	new->next = tmp;
}
