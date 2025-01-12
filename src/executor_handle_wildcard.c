/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 12:51:03 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int matches_pattern(const char *pattern, const char *str)
{
    while (*pattern && *str)
    {
        if (*pattern == '*')
        {
            while (*pattern == '*')
                pattern++;
            if (!*pattern)
                return 1;
            while (*str)
            {
                if (matches_pattern(pattern, str))
                    return 1;
                str++;
            }
            return 0;
        }
        else if (*pattern != *str) // Mismatch
        {
            return 0;
        }
        pattern++;
        str++;
    }

    return (!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str;
}

char *handle_wildcard(const char *word)
{
    DIR *dir;
    struct dirent *entry;
    char *result = malloc(1);
    size_t result_len = 0;

    if (!result)
        return NULL;

    result[0] = '\0';

    dir = opendir(".");
    if (!dir)
    {
        free(result);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (matches_pattern(word, entry->d_name))
        {
            size_t entry_len = strlen(entry->d_name);

            if (result_len > 0)
                result = append_to_result(result, &result_len, " ", 1);

            result = append_to_result(result, &result_len, entry->d_name, entry_len);
            if (!result)
            {
                closedir(dir);
                return NULL;
            }
        }
    }

    closedir(dir);

    if (result_len == 0)
    {
        free(result);
        return strdup(word);
    }

    return result;
}
