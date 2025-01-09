#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execute(t_list *script, t_list *ep);

char	*find_path(char *cmd, char **ep);

void free_matrix(char **matrix)

#endif // EXECUTOR_H
