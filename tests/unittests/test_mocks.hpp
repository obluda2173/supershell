#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include <gtest/gtest.h>
# include <gmock/gmock.h>

class SystemWrapper {
public:
	MOCK_METHOD0(mockFork, pid_t(void));
	MOCK_METHOD3(mockExecve, int(const char *pathname, char *const argv[], char *const  envp[]));
};

extern SystemWrapper* g_systemWrapper;

extern "C" pid_t mock_fork(void);
extern "C" int mock_execve(const char *pathname, char *const  argv[], char *const  envp[]);

// failure functions
pid_t ForkFailure(int error);
int ExecveFailure(const char *pathname, char *const argv[], char *const envp[]);

#endif // TEST_MOCKS_H_
