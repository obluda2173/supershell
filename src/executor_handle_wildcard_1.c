/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/16 20:21:05 by erian            ###   ########.fr       */
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
            return 0;
        pattern++;
        str++;
    }
    return (!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str;
}

char *get_dir_path(const char *word, char **pattern)
{
    char *slash = ft_strrchr(word, '/');
    if (slash)
    {
        *pattern = strdup(slash + 1);
        return ft_substr(word, 0, slash - word);
    }
    *pattern = ft_strdup(word);
    return ft_strdup(".");
}

char *create_result_buffer()
{
    char *result = malloc(1);
    if (result)
        result[0] = '\0';
    return result;
}

char *build_full_path(const char *dir_path, const char *entry_name)
{
    size_t dir_len = ft_strlen(dir_path);
    size_t entry_len = ft_strlen(entry_name);
    size_t full_len = dir_len + entry_len + 2;

    char *full_path = malloc(full_len);
    if (!full_path) return NULL;

    char *ptr = full_path;
    const char *src = dir_path;
    while (*src) *ptr++ = *src++;
    *ptr++ = '/';
    src = entry_name;
    while (*src) *ptr++ = *src++;
    *ptr = '\0';

    return full_path;
}

char *append_entry_to_result(char *result, size_t *result_len, const char *full_path)
{
    if (*result_len > 0)
        result = append_to_result(result, result_len, " ", 1);
    result = append_to_result(result, result_len, full_path, ft_strlen(full_path));
    return result;
}

char **finalize_result(char *result, char **argv)
{
    if (!result || ft_strlen(result) == 0)
    {
        free(result);
        return argv;
    }

    char **split_result = ft_split(result, ' ');
    char **expanded_argv = ft_matrix_join(argv, split_result);

    free_matrix(split_result);
    free(result);

    return expanded_argv;
}

char *process_entry(char *result, size_t *result_len, const char *dir_path, const char *entry_name)
{
    char *full_path = build_full_path(dir_path, entry_name);
    if (!full_path)
        return NULL;
    result = append_entry_to_result(result, result_len, full_path);
    free(full_path);
    return result;
}

void free_resources(DIR *dir, char *dir_path, char *pattern, char *result)
{
    if (dir) closedir(dir);
    if (dir_path) free(dir_path);
    if (pattern) free(pattern);
    if (result) free(result);
}

char **process_directory(const char *dir_path, const char *pattern, char **argv)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
        return argv; 

    char *result = create_result_buffer();
    if (!result)
    {
        closedir(dir);
        return NULL;
    }
    struct dirent *entry;
    size_t result_len = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (matches_pattern(pattern, entry->d_name))
        {
            result = process_entry(result, &result_len, dir_path, entry->d_name);
            if (!result)
            {
                free_resources(dir, NULL, NULL, result);
                return NULL;
            }
        }
    }
    closedir(dir);
    return finalize_result(result, argv);
}

char **handle_wildcard(const char *word, char **argv)
{
    char *dir_path, *pattern;

    dir_path = get_dir_path(word, &pattern);
    if (!dir_path || !pattern)
    {
        free(dir_path);
        free(pattern);
        return NULL;
    }

    char **expanded_argv = process_directory(dir_path, pattern, argv);
    free(dir_path);
    free(pattern);
    return expanded_argv;
}
