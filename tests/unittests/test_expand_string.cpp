#include "test_main.hpp"

struct expandStringParams {
	const char* str;
	const char* want;
};

class expandStringTests : public::testing::TestWithParam<expandStringParams>{};

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

TEST_P(expandStringTests, firstTest) {
	expandStringParams params = GetParam();
	t_list *ep = get_ep();
	t_data data = {ep, NULL, 0, false};

	char* got = expand_string((char*)params.str, &data);
	EXPECT_STREQ(params.want, got);
	ft_lstclear(&ep, free_env_var);
	free(got);
}

INSTANTIATE_TEST_SUITE_P(
	expandStrings,
	expandStringTests,
	testing::Values(
		expandStringParams{"hello", "hello"},
		expandStringParams{"hello2", "hello2"},
		expandStringParams{"$PATH", "path_value"},
		expandStringParams{"\"$PATH path $PATH $path\"", "path_value path path_value "},
		expandStringParams{"\"$PATH path $PATH$LOGNAME $path\"", "path_value path path_valuelogname "},
		expandStringParams{"VAR=\"$PATH path $PATH $path \"", "VAR=path_value path path_value  "},
		expandStringParams{"VAR='$PATH path $PATH $path '\"$LOGNAME $LOGNAME\"", "VAR=$PATH path $PATH $path logname logname"},
		expandStringParams{"VAR=\"$PATH path $PATH $path \"'$LOGNAME $LOGNAME'", "VAR=path_value path path_value  $LOGNAME $LOGNAME"},
		expandStringParams{"$LOGNAME$LOGNAME", "lognamelogname"},
		expandStringParams{"VAR=$LOGNAME", "VAR=logname"},
		// fuzzy tests
		expandStringParams{"\"$PATH path $PATH $path \"VAR='$LOGNAME $LOGNAME'", "path_value path path_value  VAR=$LOGNAME $LOGNAME"},
		expandStringParams{"VAR=\"\"", "VAR="},
		expandStringParams{"\"\"", ""},
		expandStringParams{"", ""},
		expandStringParams{"VAR='$PATH path $PATH $path '", "VAR=$PATH path $PATH $path "},
		expandStringParams{"$LOGNAME\"$LOGNAME\"", "lognamelogname"},
		expandStringParams{"\"$PATH\"", "path_value"},
		expandStringParams{"'$PATH'", "$PATH"},
		expandStringParams{"\"$PATH $LOGNAME\"", "path_value logname"},
		expandStringParams{"'$PATH $LOGNAME'", "$PATH $LOGNAME"},
		expandStringParams{"\"$PATH-path_$LOGNAME\"", "path_value-path_logname"},
		expandStringParams{"'$PATH-path_$LOGNAME'", "$PATH-path_$LOGNAME"},
		expandStringParams{"\"The path is $PATH and logname is $LOGNAME\"", "The path is path_value and logname is logname"},
		expandStringParams{"\"$UNSET_VAR\"", ""},
		expandStringParams{"'Unchanged $UNSET_VAR'", "Unchanged $UNSET_VAR"},
		expandStringParams{"$LOGNAME='hello'", "logname=hello"},
		expandStringParams{"$LOGNAME!", "logname!"},
		expandStringParams{"$", "$"},
		expandStringParams{"\"$\"", "$"},
		expandStringParams{"\"Just text with no variables\"", "Just text with no variables"},
		expandStringParams{"\"$$PATH\"", "$path_value"},
		expandStringParams{"\"$PATH\"'$LOGNAME'", "path_value$LOGNAME"},
		expandStringParams{"'\"$PATH\"$LOGNAME'", "\"$PATH\"$LOGNAME"},
		expandStringParams{"\"PATH$PATH\"", "PATHpath_value"},
		expandStringParams{"\"$LOGNAME/$PATH\"", "logname/path_value"},
		expandStringParams{"\"$PATH$LOGNAME\"", "path_valuelogname"},
		expandStringParams{"\"$1_PATH\"", ""},  // Assuming $1 is not set.
		expandStringParams{"'$$'", "$$"},
		expandStringParams{"\"$10$LOGUNSET\"", ""}  // Assuming $10 and $LOGNAME unset or empty.
		)
	);
