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
	env_var = new_env_var((char*)"$", (char*)"1234");
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
		expandVariableParams{"VAR=\"$PATH path $PATH $path \"", "VAR=path_value path path_value  "},
		expandVariableParams{"VAR='$PATH path $PATH $path '\"$LOGNAME $LOGNAME\"", "VAR=$PATH path $PATH $path logname logname"},
		expandVariableParams{"VAR=\"$PATH path $PATH $path \"'$LOGNAME $LOGNAME'", "VAR=path_value path path_value  $LOGNAME $LOGNAME"},
		expandVariableParams{"$LOGNAME$LOGNAME", "lognamelogname"},
		expandVariableParams{"VAR=$LOGNAME", "VAR=logname"},
		// fuzzy tests
		expandVariableParams{"\"$PATH path $PATH $path \"VAR='$LOGNAME $LOGNAME'", "path_value path path_value  VAR=$LOGNAME $LOGNAME"},
		expandVariableParams{"VAR=\"\"", "VAR="},
		expandVariableParams{"\"\"", ""},
		expandVariableParams{"", ""},
		expandVariableParams{"VAR='$PATH path $PATH $path '", "VAR=$PATH path $PATH $path "},
		expandVariableParams{"$LOGNAME\"$LOGNAME\"", "lognamelogname"},
		expandVariableParams{"\"$PATH\"", "path_value"},
		expandVariableParams{"'$PATH'", "$PATH"},
		expandVariableParams{"\"$PATH $LOGNAME\"", "path_value logname"},
		expandVariableParams{"'$PATH $LOGNAME'", "$PATH $LOGNAME"},
		expandVariableParams{"\"$PATH-path_$LOGNAME\"", "path_value-path_logname"},
		expandVariableParams{"'$PATH-path_$LOGNAME'", "$PATH-path_$LOGNAME"},
		expandVariableParams{"\"The path is $PATH and logname is $LOGNAME\"", "The path is path_value and logname is logname"},
		expandVariableParams{"\"$UNSET_VAR\"", ""},
		expandVariableParams{"'Unchanged $UNSET_VAR'", "Unchanged $UNSET_VAR"},
		expandVariableParams{"$LOGNAME='hello'", "logname=hello"},
		expandVariableParams{"$LOGNAME!", "logname!"},
		expandVariableParams{"$", "$"},
		expandVariableParams{"\"$\"", "$"},
		expandVariableParams{"\"Just text with no variables\"", "Just text with no variables"},
		expandVariableParams{"\"$$PATH\"", "$path_value"},
		expandVariableParams{"\"$PATH\"'$LOGNAME'", "path_value$LOGNAME"},
		expandVariableParams{"'\"$PATH\"$LOGNAME'", "\"$PATH\"$LOGNAME"}
// expandVariableParams{"\"PATH$PATH\"", "PATHpath_value"},
// expandVariableParams{"\"$LOGNAME/$PATH\"", "logname/path_value"},
// expandVariableParams{"\"$PATH$LOGNAME\"", "path_valuelogname"},
// expandVariableParams{"\"$1_PATH\"", ""},  // Assuming $1 is not set.
// expandVariableParams{"'$$'", "$$"},
// expandVariableParams{"\"${PATH}text\"", "path_valuetext"},
// expandVariableParams{"\"$10$LOGNAME\"", ""}  // Assuming $10 and $LOGNAME unset or empty.
		)
	);
