#ifndef EXECUTOR_EXPANSIONS_H
#define EXECUTOR_EXPANSIONS_H

#include "minishell.h"
#include "parser.h"

/* executor/expansions */
/* expand_string.c */
char	*expand_string(char *string, t_data *data);

/* expand_string_2.c */
bool	is_env_var_char(char c);
char	*advance_to_end_of_end_var(char *word);
char	*get_next_break(char *word);

/* expand_redirections_1.c */
t_list	*ignore_wildcard_in_redirection(t_redirection redirection);
t_list	*filter_entries(t_list *dir_entries, char *pattern);
void	replace_list_next_with_new_redirection(t_list *list, t_list *new_redirect);
int	set_output(t_redirection r, int fds[2], int hered_pipe[2]);
int	set_input(t_redirection r, int fds[2], int hered_pipe[2]);

/* expand_redirections_2.c */
bool	is_current_dir(char *dir_path);
void	*teardown_redirect(char *dir_path, char *pattern, t_list *dir_entries);
t_list	*new_redirection_from_entry(char *entry, t_redirection redirection);
int teardown_close_fds(int fds[2], char* err_msg);

/* sort_arguments.c */
void	sort_arguments(t_list **list);

#endif // EXECUTOR_EXPANSIONS_H
