/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_double_quotes.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:57:45 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 12:52:56 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char *handle_double_quotes(const char *word, t_data *data)
{
    size_t len = ft_strlen(word);
    char *result = malloc(1);
    size_t result_len = 0;
    size_t i = 0;

    if (!result)
        return NULL;

    result[0] = '\0';
    
    while (i < len)
    {
        if (word[i] == '$')
        {
            i++;
            if (word[i] == '?')
            {
                char *exit_status_str = ft_itoa(data->exit_status);
                size_t exit_status_len = ft_strlen(exit_status_str);
                result = append_to_result(result, &result_len, exit_status_str, exit_status_len);
                free(exit_status_str);
                if (!result)
                    return NULL;
                i++;
            }
            else
            {
                char *dollar_expansion = handle_dollar(&word[i], data);
                size_t expansion_len = ft_strlen(dollar_expansion);
                result = append_to_result(result, &result_len, dollar_expansion, expansion_len);
                free(dollar_expansion);
                if (!result)
                    return NULL;
                while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
                    i++;
            }
        }
        else
        {
            char temp[2] = {word[i++], '\0'};
            result = append_to_result(result, &result_len, temp, 1);
            if (!result)
                return NULL;
        }
    }
    return (result);
}
