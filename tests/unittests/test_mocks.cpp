#include "test_mocks.hpp"

// setup Mock ////////////////////////////////////////
// Mock class to mock systemCalls: fork

SystemWrapper* g_systemWrapper = nullptr;

// C-style function matching the PixelPutFunc signature
extern "C" pid_t mock_fork(void) {
	if (g_systemWrapper)
		return g_systemWrapper->mockFork();
	return 0;
}

// Helper function to simulate fork failure
pid_t ForkFailure(int error) {
	errno = error; // Simulate setting errno as fork would
	return -1;      // Simulate fork failure
}

extern "C" int mock_execve(const char *pathname, char *const  argv[], char *const  envp[]) {
	(void)pathname;
	(void)argv;
	(void)envp;
	if (g_systemWrapper)
		return g_systemWrapper->mockExecve(pathname, argv, envp);
	return 0;
}

int ExecveFailure(const char *pathname, char *const argv[],
                  char *const envp[]) {
	(void)pathname;
	(void)argv;
	(void)envp;
  return -1;
}
