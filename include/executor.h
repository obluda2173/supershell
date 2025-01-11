#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "libft.h"
#include "parser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* int execute_script(t_script_node *script_node, char **envp); */
int execute_script(t_script_node *script_node, char **envp);

char	*find_path(char *cmd, char **ep);
void free_matrix(char **matrix);
char *handle_dollar(const char *word);
int last_exit_status(int new_status, int update);

int execute_command(t_cmd_node cmd_node, char **envp);
char *handle_double_quotes(const char *word);

// executor_free.c
void free_matrix(char **matrix);

#endif // EXECUTOR_
