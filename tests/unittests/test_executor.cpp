#include "test_main.hpp"

using ::testing::Invoke;

t_script_node *new_script_node(char *cmd) {
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	sn->node_type = CMD_NODE;
	sn->node_data.cmd_node.cmd_token = new_token(ft_strdup(cmd), BUILTIN);
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
	sn->upstream = NULL;
	sn->downstream = NULL;
	return sn;
}

// setup Mock ////////////////////////////////////////
// Mock class to mock systemCalls: fork
class SystemWrapper {
public:
	MOCK_METHOD0(mockFork, pid_t(void));
};

SystemWrapper* g_systemWrapper = nullptr;

// C-style function matching the PixelPutFunc signature
extern "C" pid_t mock_fork(void) {
	if (g_systemWrapper)
		return g_systemWrapper->mockFork();
	return 0;
}

// Helper function to simulate fork failure
pid_t ForkFailure() {
	errno = EAGAIN; // Simulate setting errno as fork would
	return -1;      // Simulate fork failure
}

struct ExecutorTestsParams {
	const char* cmd;
	int want_return;
	const char* want_stdout;
};

class ExecutorTestSuite : public::testing::TestWithParam<ExecutorTestsParams>{};

TEST(ExecutorTestSuite, ErrorTestsFork){
	////////////////////////////////////////////
	// mock stuff
	SystemWrapper sysMock;
	g_systemWrapper = &sysMock;
	EXPECT_CALL(sysMock, mockFork()).WillOnce(Invoke(&ForkFailure));
	t_system_calls system_calls = {mock_fork};
	////////////////////////////////////////////

	////////////////////////////////////////////
	// environment
	std::string path_env = "PATH=" + std::string(std::getenv("PATH"));
	char* envp[] = {
		(char*)path_env.c_str(),
		nullptr // Null terminator for the array
	};
	////////////////////////////////////////////

	t_script_node *script = new_script_node((char*)"ls");
	testing::internal::CaptureStderr();
	int got_return = execute_script(script, envp, system_calls);


	EXPECT_EQ(1, got_return);
	std::string got = testing::internal::GetCapturedStderr();
	EXPECT_STREQ("fork: Resource temporarily unavailable\n", got.c_str());
	free_script_node(script);
}

char **get_envp() {
	char** envp = (char**)malloc(sizeof(char*) * 2);
	envp[0] = (char*)("PATH=" + std::string(std::getenv("PATH"))).c_str();
	envp[1] = NULL;
	return envp;

}


TEST_P(ExecutorTestSuite, ErrorTests) {
	ExecutorTestsParams params = GetParam();
	t_system_calls sc = {fork};

	////////////////////////////////////////////
	// environment
	std::string path_env = "PATH=" + std::string(std::getenv("PATH"));
	char* envp[] = {
		(char*)path_env.c_str(),
		nullptr // Null terminator for the array
	};
	// char** envp = get_envp();
	////////////////////////////////////////////

	t_script_node *script = new_script_node((char*)params.cmd);
	// testing::internal::CaptureStderr();
	int got_return = execute_script(script, envp, sc);
	EXPECT_EQ(127, got_return);
	// std::string got = testing::internal::GetCapturedStderr();
	// EXPECT_STREQ("Command not found: random_cmd\n", got.c_str());

	// free(envp);
	free_script_node(script);
}

INSTANTIATE_TEST_SUITE_P(
	ExecutorTests,
	ExecutorTestSuite,
	testing::Values(
		ExecutorTestsParams{"random_cmd", 127, "Command not found: random_cmd\n"}
// 		ExecutorTestParams{{new_argument("Hello", LITERAL)}, "Hello\n", 0},
// 		ExecutorTestParams{{new_argument("Hello", LITERAL), new_argument("World", LITERAL)}, "Hello World\n", 0}
		)
	);
