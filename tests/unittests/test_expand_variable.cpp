#include "test_main.hpp"

struct expandVariableParams {
	const char* word;
	const char* want;
};

class expandVariableTests : public::testing::TestWithParam<expandVariableParams>{};

t_list *get_ep() {
	t_list *ep = NULL;
	t_env_var *env_var = new_env_var((char*)"PATH", (char*)"path_value");
	ft_lstadd_back(&ep, ft_lstnew(env_var));
	env_var = new_env_var((char*)"LOGNAME", (char*)"logname");
	ft_lstadd_back(&ep, ft_lstnew(env_var));
	return ep;
}

TEST_P(expandVariableTests, firstTest) {
	expandVariableParams params = GetParam();
	t_list *ep = get_ep();
	t_data data = {ep, NULL, 0, false};

	char* got = expand_variables((char*)params.word, &data);
	EXPECT_STREQ(params.want, got);
	ft_lstclear(&ep, free_env_var);
	free(got);
}

INSTANTIATE_TEST_SUITE_P(
	expandVariables,
	expandVariableTests,
	testing::Values(
		expandVariableParams{"hello", "hello"},
		expandVariableParams{"hello2", "hello2"},
		expandVariableParams{"$PATH", "path_value"},
		expandVariableParams{"\"$PATH path $PATH $path\"", "path_value path path_value "},
		expandVariableParams{"\"$PATH path $PATH$LOGNAME $path\"", "path_value path path_valuelogname "},
		expandVariableParams{"VAR=\"$PATH path $PATH $path\"", "VAR=path_value path path_value "}
		// expandVariableParams{"VAR1=\"$PATH path $PATH $path\"", "VAR1=path_value path path_value "}
		)
	);
