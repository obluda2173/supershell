/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:27:01 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 14:56:43 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"

int     matches_pattern(const char *pattern, const char *str)
{
    while (*pattern && *str)
    {
        if (*pattern == '*')
        {
            while (*pattern == '*')
                pattern++;
            if (!*pattern)
                return (1);
            while (*str)
            {
                if (matches_pattern(pattern, str))
                    return (1);
                str++;
            }
            return (0);
        }
        else if (*pattern != *str)
            return (0);
        pattern++;
        str++;
    }
    return ((!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str);
}

char    *build_full_path(const char *dir_path, const char *entry_name)
{
    char        *full_path;
    char        *ptr;
    const char  *src = dir_path;

    if (!ft_strcmp(dir_path, ""))
        return (ft_strdup(entry_name));
    full_path = malloc(ft_strlen(dir_path) + ft_strlen(entry_name) + 2);
    if (!full_path)
        return (NULL);
    ptr = full_path;
    while (*src)
        *ptr++ = *src++;
    *ptr++ = '/';
    src = entry_name;
    while (*src)
        *ptr++ = *src++;
    *ptr = '\0';
    return (full_path);
}
