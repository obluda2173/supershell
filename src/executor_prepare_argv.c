/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_prepare_argv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:30:10 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 15:51:41 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char     **initialize_argv(char *cmd_path, size_t count)
{
    char    **argv;

    argv = malloc(sizeof(char *) * (count + 2));
    if (!argv)
        return (NULL);
    argv[0] = ft_strdup(cmd_path);
    if (!argv[0])
        return (NULL);
    return (argv);
}

static char     **fill_argv(t_list *arguments, char **argv, size_t i)
{
    t_list      *head;
    char        *arg;
    t_argument  *argument;

    head = arguments;
    arg = NULL;
    while (head)
    {
        argument = (t_argument *)head->content;
        arg = ft_strdup(argument->word);
        if (!arg)
            return (NULL);
        argv[i++] = arg;
        head = head->next;
    }
    argv[i] = NULL;
    return (argv);
}

char    **list_to_argv(t_list *arguments, char *cmd_path)
{
    size_t  count;
    char    **argv;

    count = ft_lstsize(arguments);
    argv = initialize_argv(cmd_path, count);
    if (!argv)
        return (NULL);
    return (fill_argv(arguments, argv, 1));
}
