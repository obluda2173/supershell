#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "libft.h"
#include "parser.h"
#include "minishell.h"


/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */
// executor.c
int execute_script(t_script_node *script_node, char **envp, t_data *data);

// executor_cmd_path.c
char	*find_path(char *cmd, char **ep);

// executor_handle_double_quotes.c
char *handle_double_quotes(const char *word, t_data *data);

// executor_handle_dollar.c
char *handle_dollar(const char *word, t_data *data);

// executor_handle_utils.c
char *handle_env_expansion(const char *var_name);
char *expand_variable(const char *str, size_t *i);
char *append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len);

// executor_handle_wildcard.c
char **handle_wildcard(const char *word, char **argv);

// executer_free.c
void free_matrix(char **matrix);

// execute_cmd.c
int execute_command(t_cmd_node cmd_node, char **envp, t_data *data);

// executor_matrix_manipulation.c
size_t ft_matrix_size(char **matrix);
char **ft_matrix_join(char **matrix1, char **matrix2);


#endif // EXECUTOR_
