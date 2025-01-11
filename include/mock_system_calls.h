#ifndef MOCK_SYSTEM_CALLS_H
#define MOCK_SYSTEM_CALLS_H

typedef int (*t_fork_func)(void);
typedef int (*t_execve_func)(const char *pathname, char *const argv[], char *const envp[]);

typedef struct s_system_calls {
	t_fork_func fork;
	t_execve_func execve;
} t_system_calls;

#endif // MOCK_SYSTEM_CALLS_H
