#include "test_main.hpp"

INSTANTIATE_TEST_SUITE_P(
	ParserTestsErrorsRedirections,
	ParserTestSuite,
	testing::Values(
		ParserTestParams{0, ERROR_TEST, {
				new_token("<<", HERE_DOC),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{1, ERROR_TEST, {
				new_token("<<", HERE_DOC),
				new_token("line1\nline2", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{2, ERROR_TEST, {
				new_token(">", REDIRECT_APPEND),
				new_token("echo", BUILTIN),
				new_token("Hello", DOUBLE_QUOTE),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{3, ERROR_TEST, {
				new_token("-1>>", REDIRECT_APPEND),
				new_token("LOGNAME", DOLLAR),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{4, ERROR_TEST, {
				new_token("asdf>>", REDIRECT_APPEND),
				new_token("LOGNAME", DOLLAR),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{5, ERROR_TEST, {
				new_token(">>", REDIRECT_APPEND),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{6, ERROR_TEST,{
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{7, ERROR_TEST,{
				new_token("cat", BUILTIN),
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{8, ERROR_TEST,{
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),

			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{9, ERROR_TEST,{
				new_token("echo", BUILTIN),
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"), {})}},
		ParserTestParams{10, ERROR_TEST,{}, {new_test_script_node(ERROR_NODE, {}, new_error_node("no tokens"), {})}}
		)
	);
