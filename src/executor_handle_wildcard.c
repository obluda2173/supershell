/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/16 17:45:25 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int matches_pattern(const char *pattern, const char *str)
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
        else if (*pattern != *str)
        {
            return 0;
        }
        pattern++;
        str++;
    }

    return (!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str;
}

char **handle_wildcard(const char *word, char **argv)
{
    DIR *dir;
    struct dirent *entry;
    char *dir_path = NULL;
    char *pattern = NULL;
    char *slash = ft_strrchr(word, '/');
    char **expanded_argv = NULL;

    if (slash)
    {
        dir_path = ft_substr(word, 0, slash - word);
        pattern = strdup(slash + 1);
    }
    else
    {
        dir_path = ft_strdup(".");
        pattern = ft_strdup(word);
    }
    if (!dir_path || !pattern)
    {
        free(dir_path);
        free(pattern);
        return NULL;
    }
    dir = opendir(dir_path);
    if (!dir)
    {
        free(dir_path);
        free(pattern);
        return argv;
    }
    size_t result_len = 0;
    char *result = malloc(1);
    if (!result)
    {
        closedir(dir);
        free(dir_path);
        free(pattern);
        return NULL;
    }
    result[0] = '\0';
    while ((entry = readdir(dir)) != NULL)
    {
        if (matches_pattern(pattern, entry->d_name))
        {
            size_t entry_len = ft_strlen(entry->d_name);
            size_t full_path_len = ft_strlen(dir_path) + entry_len + 2; // +1 for '/' and +1 for '\0'
            char *full_path = malloc(full_path_len);
            if (!full_path)
            {
                free(result);
                closedir(dir);
                free(dir_path);
                free(pattern);
                return NULL;
            }

            // Manual string concatenation using `while`
            char *ptr = full_path;

            const char *dp = dir_path;
            while (*dp)
            {
                *ptr = *dp;
                ptr++;
                dp++;
            }

            // Add '/' separator
            *ptr = '/';
            ptr++;

            const char *en = entry->d_name;
            while (*en)
            {
                *ptr = *en;
                ptr++;
                en++;
            }

            // Null-terminate the string
            *ptr = '\0';

            if (result_len > 0)
                result = append_to_result(result, &result_len, " ", 1);

            result = append_to_result(result, &result_len, full_path, ft_strlen(full_path));
            free(full_path);
            if (!result)
            {
                closedir(dir);
                free(dir_path);
                free(pattern);
                return NULL;
            }
        }
    }

    closedir(dir);
    free(dir_path);
    free(pattern);

    if (result_len == 0)
    {
        free(result);
        return argv;
    }

    char **split_result = ft_split(result, ' ');
    expanded_argv = ft_matrix_join(argv, split_result);

    free_matrix(split_result);
    free(result);

    return expanded_argv;
}
