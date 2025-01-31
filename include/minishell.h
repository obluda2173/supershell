/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:42:49 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/31 12:06:30 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lexer.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

/* ************************************************************************** */
/* global variables
 * */
/* ************************************************************************** */

# include <signal.h>

extern volatile sig_atomic_t	g_signal_received;

/* ************************************************************************** */
/* structures                                                               * */
/* ************************************************************************** */

typedef struct s_env_var
{
	char						*key;
	char						*value;
}								t_env_var;

// structure for main data
typedef struct s_data
{
	t_list						*ep;
	char						*line;
	int							exit_status;
	bool						exit;
}								t_data;

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

/* data.c */
t_data							*init(char **ep);
void							free_data(t_data *data);

/* env_var.c */
t_data							*set_env_vars(char **ep, t_data *data);
t_env_var						*new_env_var(char *key, char *value);
void							free_env_var(void *content);

/* helpers.c */
void							free_char_array(char **matrix);
int								error_fork(void);

/* heredoc.c */
int								heredoc_loop(t_dllist **tokens, t_data *data);

/* heredoc_1.c */
t_dllist						*search_heredoc(t_dllist *tokens);
char							*add_quotes(char *str);
t_dllist						*create_heredoc_token(char *heredoc_input,
									bool quoted_delimiter);
char							*extract_delimiter(t_dllist **heredoc_token);

/* input.c */
char							*minishell_input(t_data *data);

/* readline.c */
char							*read_line_from_fd(int read_fd);

/* repl.c */
int								repl(t_data *data);

/* signals.h */
void							handle_signals(int signum);
void							handle_signals_2(int signum);

#endif
