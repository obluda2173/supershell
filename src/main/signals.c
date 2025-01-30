#include "minishell.h"

void	handle_signals_2(int signum)
{
	if (signum == SIGINT)
		signal_received = 1;
}

void	handle_signals(int signum)
{
	if (signum == SIGINT) {
		rl_replace_line("", 0);
		ft_putendl_fd("", STDOUT_FILENO);
		rl_on_new_line();
		rl_redisplay();
		signal_received = 1;
	}
}
