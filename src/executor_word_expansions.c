/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_word_expansions.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:57:45 by erian             #+#    #+#             */
/*   Updated: 2025/01/10 17:14:10 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char *handle_env_expansion(const char *var_name)
{
    char *value = getenv(var_name);
    if (!value)
        return (ft_strdup(""));
    return (ft_strdup(value));
}

static char *expand_variable(const char *str, size_t *i)
{
    size_t start = *i;
    while (ft_isalnum(str[*i]) || str[*i] == '_')
        (*i)++;
    char *var_name = ft_strndup(&str[start], *i - start);
    char *var_value = handle_env_expansion(var_name);
    free(var_name);
    return var_value ? var_value : ft_strdup("");
}


char *handle_double_quotes(const char *word)
{
    size_t len = strlen(word);
    char *result = malloc(1);
    size_t result_len = 0;
    size_t i = 0;

    if (!result)
        return NULL;

    while (i < len)
    {
        if (word[i] == '$')
        {
            i++;
            // if (word[i] == '?') // Handle $?
            // {
            //     char *exit_status = handle_exit_status_expansion();
            //     size_t exit_status_len = strlen(exit_status);
            //     result = realloc(result, result_len + exit_status_len + 1);
            //     if (!result)
            //         return NULL;
            //     strcpy(result + result_len, exit_status);
            //     result_len += exit_status_len;
            //     free(exit_status);
            //     i++;
            // }
            if (ft_isalnum(word[i]) || word[i] == '_')
            {
                char *var_value = expand_variable(word, &i);
                size_t var_value_len = strlen(var_value);
                result = realloc(result, result_len + var_value_len + 1);
                if (!result)
                    return NULL;
                strcpy(result + result_len, var_value);
                result_len += var_value_len;
                free(var_value);
            }
            else
            {
                result = realloc(result, result_len + 2);
                if (!result)
                    return NULL;
                result[result_len++] = '$';
                if (word[i])
                    result[result_len++] = word[i++];
            }
        }
        else
        {
            result = realloc(result, result_len + 2);
            if (!result)
                return NULL;
            result[result_len++] = word[i++];
        }
    }

    result[result_len] = '\0';
    return result;
}
