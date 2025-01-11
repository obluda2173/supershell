#include "test_main.hpp"
#include "test_mocks.hpp"

using ::testing::_;

TEST(ExecutorTestSuite, ErrorTestsExecve) {
	////////////////////////////////////////////
	// mock stuff
	SystemWrapper sysMock;
	g_systemWrapper = &sysMock;
	EXPECT_CALL(sysMock, mockExecve(_, _, _))
		.WillOnce(
			Invoke([](const char* pathname, char* const argv[], char* const envp[]) {
				return ExecveFailure(pathname, argv, envp); // Provide the desired argument
	}));

	// Example usage
    const char* path = "/bin/nonexistent";
    char* args[] = {nullptr};
    char* env[] = {nullptr};
    int result = sysMock.mockExecve(path, args, env);
    ASSERT_EQ(result, -1); // Ensure ExecveFailure is enforced

	t_system_calls sc = {mock_fork, mock_execve};
	(void)sc;

}

TEST(ExecutorTestSuite, ErrorTestsFork) {
	////////////////////////////////////////////
	// mock stuff
	SystemWrapper sysMock;
	g_systemWrapper = &sysMock;
	EXPECT_CALL(sysMock, mockFork()).WillOnce(Invoke([]() {
		return ForkFailure(EAGAIN); // Provide the desired argument
	}));

	t_system_calls sc = {mock_fork, mock_execve};
	char** envp = get_envp();

	t_script_node *script = new_script_node((char*)"ls");
	testing::internal::CaptureStderr();
	int got_return = execute_command(script->node_data.cmd_node, envp, sc);


	EXPECT_EQ(1, got_return);
	std::string got = testing::internal::GetCapturedStderr();
	EXPECT_STREQ("fork: Resource temporarily unavailable\n", got.c_str());

	free_matrix(envp);
	free_script_node(script);
}


TEST_P(ExecutorTestSuite, ErrorTests) {
	// setup
	ExecutorTestsParams params = GetParam();
	t_system_calls sc = {fork, execve};
	char** envp = get_envp();
	t_script_node *script = new_script_node((char*)params.cmd);

	// run
	testing::internal::CaptureStderr();
	testing::internal::CaptureStdout();
	int got_return = execute_script(script, envp, sc);

	// compare
	EXPECT_EQ(params.want_return, got_return);
	std::string got_stderr = testing::internal::GetCapturedStderr();
	std::string got_stdout = testing::internal::GetCapturedStdout();
	EXPECT_STREQ(params.want_stderr, got_stderr.c_str());

	free_matrix(envp);
	free_script_node(script);
}

INSTANTIATE_TEST_SUITE_P(
	ExecutorTests,
	ExecutorTestSuite,
	testing::Values(
		ExecutorTestsParams{"random_cmd", 127, "", "Command not found: random_cmd\n"},
		ExecutorTestsParams{"ls", 0, "",""}
		)
	);
