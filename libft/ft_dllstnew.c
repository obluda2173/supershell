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

t_dllist	*ft_dllstnew(void *content)
{
	t_dllist	*new;

	new = (t_dllist *)malloc(sizeof(t_dllist));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
