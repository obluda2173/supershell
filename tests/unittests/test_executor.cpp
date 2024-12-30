#include "test_main.hpp"

t_list *create_script(std::vector<t_argument> args) {
	(void)args;
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	t_list *script = NULL;
	sn->token = new_token(ft_strdup("echo"), BUILTIN);
	sn->type = CMD_NODE;
	sn->argument_count = (int)args.size();
	if (sn->argument_count == 0)
		sn->arguments = NULL;
	else
		sn->arguments = (t_argument**)malloc(sizeof(t_argument*) * sn->argument_count);

	int count = 0;
	while (count < sn->argument_count) {
		sn->arguments[count] = (t_argument*)malloc(sizeof(t_argument));
		sn->arguments[count]->literal = ft_strdup(args[count].literal);
		count++;
	}

	ft_lstadd_back(&script, ft_lstnew(sn));
	return script;
}


TEST(ExecutorTests, FirstTest) {
	t_list *script = NULL;
	std::vector<t_argument> args = {};
	script = create_script(args);

	testing::internal::CaptureStdout();
	EXPECT_EQ(0, execute(script));
	std::string got = testing::internal::GetCapturedStdout();
	EXPECT_STREQ("\n", got.c_str());
	ft_lstclear(&script, free_script_node);

	args = {new_argument("Hello")};
	script = create_script(args);
	testing::internal::CaptureStdout();
	EXPECT_EQ(0, execute(script));
	got = testing::internal::GetCapturedStdout();
	EXPECT_STREQ("Hello\n", got.c_str());
	ft_lstclear(&script, free_script_node);

	args = {new_argument("Hello"), new_argument("World!")};
	script = create_script(args);
	testing::internal::CaptureStdout();
	EXPECT_EQ(0, execute(script));
	got = testing::internal::GetCapturedStdout();
	EXPECT_STREQ("Hello World!\n", got.c_str());
	ft_lstclear(&script, free_script_node);
}
