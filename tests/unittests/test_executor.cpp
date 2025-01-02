#include "test_main.hpp"

t_list *create_script(std::vector<t_argument> args) {
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	t_list *script = NULL;
	sn->node_data.cmd_node.cmd_token = new_token(ft_strdup("echo"), BUILTIN);
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
	for (auto want_arg : args) {
		t_argument *arg = (t_argument*)malloc(sizeof(t_argument));
		arg->literal = ft_strdup(want_arg.literal);
		ft_lstadd_back(&sn->node_data.cmd_node.arguments, ft_lstnew(arg));
	}
	ft_lstadd_back(&script, ft_lstnew(sn));
	return script;
}

struct ExecutorTestParams {
	std::vector<t_argument> args;
	std::string want_output;
	int want_return;
};

class ExecutorTestSuite : public::testing::TestWithParam<ExecutorTestParams>{};

TEST_P(ExecutorTestSuite, ExecutorTests) {
	ExecutorTestParams params = GetParam();
	t_list *script = create_script(params.args);

	testing::internal::CaptureStdout();
	EXPECT_EQ(params.want_return, execute(script));
	std::string got = testing::internal::GetCapturedStdout();
	EXPECT_STREQ(params.want_output.c_str(), got.c_str());
	ft_lstclear(&script, free_script_node);
}


INSTANTIATE_TEST_SUITE_P(
	ExecutorTests, ExecutorTestSuite,
	testing::Values(
		ExecutorTestParams{{}, "\n", 0},
		ExecutorTestParams{{new_argument("Hello", LITERAL)}, "Hello\n", 0},
		ExecutorTestParams{{new_argument("Hello", LITERAL), new_argument("World", LITERAL)}, "Hello World\n", 0}
		));
