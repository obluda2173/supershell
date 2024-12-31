#include "executor.h"
#include "libft.h"
#include "parser.h"

int execute(t_list *script) {
	t_script_node sn = *(t_script_node*)script->content;

	int count = 0;
	while (count < sn.argument_count) {
		if (count > 0)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(sn.arguments[count]->literal, STDOUT_FILENO);
		count++;
	}

	ft_putendl_fd("", STDOUT_FILENO);
	return 0;
}
