#include "executor.h"
#include "parser.h"
#include "libft.h"

int execute_script(t_script_node *script, char** envp) {
	(void)envp;
	t_list* args = script->node_data.cmd_node.arguments;
	while (args) {
		t_argument arg = *(t_argument*)args->content;
		ft_putstr_fd(arg.word, STDOUT_FILENO);
		args = args->next;
		if (args)
			ft_putstr_fd(" ", STDOUT_FILENO);
	}

	ft_putendl_fd("", STDOUT_FILENO);
	return 0;
}
