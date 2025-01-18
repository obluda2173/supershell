/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 14:53:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
