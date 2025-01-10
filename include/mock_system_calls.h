#ifndef MOCK_SYSTEM_CALLS_H
#define MOCK_SYSTEM_CALLS_H

typedef int (*t_fork_func)(void);

typedef struct s_system_calls {
	t_fork_func fork;
} t_system_calls;

#endif // MOCK_SYSTEM_CALLS_H
