#include "test_main.hpp"
#include "test_mocks.hpp"


TEST_P(ExecutorTestSuite, ErrorTests) {
	// setup
	ExecutorTestsParams params = GetParam();
	char** envp = get_envp();
	t_data *data = init(envp);
	t_script_node *script = new_script_node((char*)params.cmd, params.type);

	// run
	testing::internal::CaptureStderr();
	testing::internal::CaptureStdout();
	int got_return = execute_script(script,  data);

	// compare
	EXPECT_EQ(params.want_return, got_return);
	std::string got_stderr = testing::internal::GetCapturedStderr();
	std::string got_stdout = testing::internal::GetCapturedStdout();
	EXPECT_STREQ(params.want_stderr, got_stderr.c_str());

	free_data(data);
	free_char_array(envp);
	free_script_node(script);
}

INSTANTIATE_TEST_SUITE_P(
	ExecutorTests,
	ExecutorTestSuite,
	testing::Values(
		ExecutorTestsParams{"random_cmd", WORD, 127, "", "Command not found: random_cmd\n"},
		ExecutorTestsParams{"ls", WORD, 0, "",""}
		)
	);
