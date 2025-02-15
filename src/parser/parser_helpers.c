/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:52/55 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 14:52:55 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	copy_token(t_token token)
{
	t_token	copy;

	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return (copy);
}

bool	is_redirection_token(t_token t)
{
	if (t.type == REDIRECT_OUT || t.type == REDIRECT_IN
		|| t.type == REDIRECT_APPEND || t.type == HERE_DOC)
		return (true);
	return (false);
}
