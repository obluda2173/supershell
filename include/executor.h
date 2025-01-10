#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "libft.h"
#include "parser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mock_system_calls.h"

/* int execute_script(t_script_node *script_node, char **envp); */
int execute_script(t_script_node *script_node, char **envp, t_system_calls sc);

char	*find_path(char *cmd, char **ep);
void free_matrix(char **matrix);

int execute_command(t_cmd_node *cmd_node, char **envp, t_system_calls sc);
char *handle_double_quotes(const char *word);

// executor_free.c
void free_matrix(char **matrix);

#endif // EXECUTOR_
