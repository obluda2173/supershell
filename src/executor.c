#include "executor.h"
#include "libft.h"
#include "parser.h"

int execute(t_list *script) {
	t_script_node sn = *(t_script_node*)script->content;

	t_list* head = sn.node_data.cmd_node.arguments;
	while (head) {
		t_argument arg = *(t_argument*)head->content;
		ft_putstr_fd(arg.word, STDOUT_FILENO);
		head = head->next;
		if (head)
			ft_putstr_fd(" ", STDOUT_FILENO);
	}

	ft_putendl_fd("", STDOUT_FILENO);
	return 0;
}
