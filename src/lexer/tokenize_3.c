/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:23:51 by erian             #+#    #+#             */
/*   Updated: 2025/02/01 10:24:43 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static bool	is_builtin(char *str)
{
	return (!ft_strcmp(str, "echo\0")
		|| !ft_strcmp(str, "cd\0")
		|| !ft_strcmp(str, "pwd\0")
		|| !ft_strcmp(str, "export\0")
		|| !ft_strcmp(str, "unset\0")
		|| !ft_strcmp(str, "env\0")
		|| !ft_strcmp(str, "exit\0"));
}

static t_token_type	assign_operator_type(char *str)
{
	if (!ft_strcmp(str, "||\0"))
		return (OR);
	if (!ft_strcmp(str, "&&\0"))
		return (AND);
	if (!ft_strcmp(str, "|\0"))
		return (PIPE);
	if (!ft_strncmp(str, "<<", 2))
		return (HERE_DOC);
	if (!ft_strcmp(str, ">>\0"))
		return (REDIRECT_APPEND);
	if (!ft_strcmp(str, "<\0"))
		return (REDIRECT_IN);
	if (!ft_strcmp(str, ">\0"))
		return (REDIRECT_OUT);
	if (!ft_strcmp(str, "(\0"))
		return (LPAREN);
	if (!ft_strcmp(str, ")\0"))
		return (RPAREN);
	return (WORD);
}

static bool	identify_wildcard(char *str)
{
	if (ft_strchr(str, '*'))
	{
		if (in_quotes(str, ft_strchr(str, '*') - str))
			return (false);
		return (true);
	}
	return (false);
}

static t_token_type	assign_general_type(char *str)
{
	if (!str)
		return (END_OF_FILE);
	if (identify_wildcard(str))
		return (WILDCARD);
	if (ft_strchr(str, ';') || ft_strchr(str, '\\'))
		return (INVALID);
	if (is_builtin(str))
		return (BUILTIN);
	return (WORD);
}

t_token_type	assign_type(char *str)
{
	t_token_type	type;

	type = assign_operator_type(str);
	if (type != WORD)
		return (type);
	type = assign_general_type(str);
	return (type);
}
