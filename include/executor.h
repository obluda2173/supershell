#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

int execute_script(t_script_node *script, char** envp);

#endif // EXECUTOR_H
