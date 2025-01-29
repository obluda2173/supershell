#ifndef EXECUTOR_EXPANSIONS_H
#define EXECUTOR_EXPANSIONS_H

#include "minishell.h"
#include "parser.h"

/* executor/expansions */

// handle_dollar_1.c
char	*handle_dollar(const char *word, t_data *data);

// handle_double_quotes.c
char    *handle_double_quotes(const char *word,  t_data *data);

// handle_dollar_2.c
char	*handle_exit_status(size_t *i, t_data *data);
char	*handle_env_var(size_t *i, const char *word, t_data *data);
char	*handle_other_char(size_t *i, const char *word);
char	*append_and_check(char *result, size_t *result_len, char *str);

// handle_utils.c
char *handle_env_expansion(const char *var_name, t_data *data);
char	*expand_variable(const char *str, size_t *i, t_data *data);
char	*append_to_result(char *result, const char *addition, size_t addition_len);

/* expand_arguments.c */
int	expand_arguments(t_cmd_node *cmd_node, t_data *data);
t_argument	*get_new_argument(char *entry, char *dir_path);

/* expand_arguments_1.c */
t_list	*ignore_wildcard_in_argument(char *dir_path, char *pattern);
t_list	*create_wildcard_arguments(t_list *dir_entries, char *dir_path, char *pattern);


/* expand_redirections.c */
int	expand_redirections(t_cmd_node *cmd_node, t_data *data);
int	set_redirections(t_list **redirections, int fds[2]);

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

/* expand_string.c */
char	*expand_string(char *string, t_data *data);

/* expand_string_2.c */
bool	is_env_var_char(char c);
char	*advance_to_end_of_end_var(char *word);
char	*get_next_break(char *word);

/* sort_arguments.c */
void	sort_arguments(t_list **list);

/* wildcards_dir_entries.c */
char	*get_dir_path(char* path);
t_list	*get_dir_entries(char *dir_path);
bool	hidden_n_star(char *entry, char *pattern);
t_list	*parse_dir_entries(t_list *dir_entries, char *dir_path, char *pattern);
char *build_full_path(const char *dir_path, const char *entry_name);

/* wildcards_patterns.c */
char	*get_pattern(char* word);
int	matches_pattern(const char *pattern, const char *str);

#endif // EXECUTOR_EXPANSIONS_H
