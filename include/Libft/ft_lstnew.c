/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:41:28 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 11:35:00 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_ep	*ft_lstnew(void *content)
{
	t_ep	*node;

	node = (t_ep *)malloc(sizeof(t_ep));
	if (!node)
		return (NULL);
	node->value = ft_strdup(content);;
	node->next = NULL;
	return (node);
}
