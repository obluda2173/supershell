/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:07:50 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 16:43:17 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
// #include "executor_kay.h"

char	*expand_from_single_quote(char *word, t_data *data)
{
	char	*after;
	char	*prepend;
	char	*result;

	after = expand_string(ft_strchr(word + 1, '\'') + 1, data);
	prepend = malloc(sizeof(char) * (ft_strchr(word + 1, '\'') - word));
	ft_strlcpy(prepend, word + 1, (ft_strchr(word + 1, '\'') - word));
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return (result);
}

char	*expand_from_double_quote(char *word, t_data *data)
{
	char	*after;
	char	*prepend;
	char	*result;

	after = expand_string(ft_strchr(word + 1, '\"') + 1, data);
	prepend = handle_double_quotes(word + 1, data);
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return (result);
}

char	*expand_from_dollar(char *word, t_data *data)
{
	char	*next;
	char	*after;
	char	*prepend;
	char	*result;

	next = advance_to_end_of_end_var(word + 1);
	if (next)
		after = expand_string(next, data);
	else
		after = ft_strdup("");
	prepend = handle_dollar(word + 1, data);
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return (result);
}

char	*expand_from_here(char *word, t_data *data)
{
	char	*next;
	char	*after;
	char	*result;

	after = NULL;
	next = get_next_break(word);
	if (!next)
		result = ft_strdup(word);
	else
	{
		after = expand_string(next, data);
		*next = '\0';
		result = ft_strjoin(word, after);
	}
	free(after);
	return (result);
}

char	*expand_string(char *string, t_data *data)
{
	char	*result;

	string = ft_strdup(string);
	result = NULL;
	if (*string == '\'')
		result = expand_from_single_quote(string, data);
	if (*string == '\"')
		result = expand_from_double_quote(string, data);
	if (*string == '$')
		result = expand_from_dollar(string, data);
	if (!result)
		result = expand_from_here(string, data);
	free(string);
	return (result);
}
