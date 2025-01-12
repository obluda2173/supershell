/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_word_expansions.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:57:45 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 11:35:02 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char *handle_env_expansion(const char *var_name)
{
    char *value = getenv(var_name);
    if (value)
        return (ft_strdup(value));
    return (ft_strdup(""));
}

static char *expand_variable(const char *str, size_t *i)
{
    size_t start = *i;
    while (ft_isalnum(str[*i]) || str[*i] == '_')
        (*i)++;
    char *var_name = ft_strndup(&str[start], *i - start);
    char *var_value = handle_env_expansion(var_name);
    free(var_name);
    if (var_value)
        return (var_value);
    return (ft_strdup(""));
}

static char *append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len)
{
    char *new_result = malloc(*result_len + addition_len + 1);
    if (!new_result)
    {
        free(result);
        return NULL;
    }
    if (result)
    {
        ft_memcpy(new_result, result, *result_len);
        free(result);
    }
    ft_memcpy(new_result + *result_len, addition, addition_len);
    *result_len += addition_len;
    new_result[*result_len] = '\0';
    return new_result;
}

char *handle_dollar(const char *word, t_data *data)
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
        else if (ft_isalnum(word[i]) || word[i] == '_')
        {
            char *var_value = expand_variable(word, &i);
            size_t var_value_len = ft_strlen(var_value);
            result = append_to_result(result, &result_len, var_value, var_value_len);
            free(var_value);
            if (!result)
                return NULL;
        }
        else
        {
            char temp[2] = {word[i++], '\0'};
            result = append_to_result(result, &result_len, temp, 1);
            if (!result)
                return NULL;
        }
    }
    return result;
}


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
    return result;
}
