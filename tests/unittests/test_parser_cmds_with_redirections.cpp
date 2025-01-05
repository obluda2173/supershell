#include "test_main.hpp"

INSTANTIATE_TEST_SUITE_P(
	ParserTestsErrorsRedirections,
	ParserTestSuite,
	testing::Values(
		ParserTestParams{
			{
				new_token("<<", HERE_DOC),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{
			{
				new_token("<<", HERE_DOC),
				new_token("line1\nline2", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token(">", REDIRECT_APPEND),
				new_token("echo", BUILTIN),
				new_token("Hello", DOUBLE_QUOTE),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("-1>>", REDIRECT_APPEND),
				new_token("LOGNAME", DOLLAR),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("asdf>>", REDIRECT_APPEND),
				new_token("LOGNAME", DOLLAR),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token(">>", REDIRECT_APPEND),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("cat", BUILTIN),
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),

			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{}, {new_test_script_node(ERROR_NODE, {}, new_error_node("no tokens"))}}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	ParserTestsCmdsWithRedirections, ParserTestSuite,
	testing::Values(
		ParserTestParams{
		{
		new_token("cat", WORD),
		new_token("<<", HERE_DOC),
		new_token("line1\n$PATH\nline2", DOUBLE_QUOTE),
		new_token("3>", REDIRECT_OUT), // here I added the file descriptor 3 before the command
		new_token("line3 $PATH line4", DOUBLE_QUOTE),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("cat", WORD), {},
								   {
								   new_redirection(0, HERED, "line1\n$PATH\nline2", DOUBLE_QUOTE_STR),
								   new_redirection(3, OUT, "line3 $PATH line4", DOUBLE_QUOTE_STR),

							   })},
				{})}},
		ParserTestParams{
		{
		new_token("cat", WORD),
		new_token("<<", HERE_DOC),
		new_token("line1\n$PATH\nline2", DOUBLE_QUOTE),
		new_token(">", REDIRECT_OUT),
		new_token("line3 $PATH line4", DOUBLE_QUOTE),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("cat", WORD), {},
								   {
								   new_redirection(0, HERED, "line1\n$PATH\nline2", DOUBLE_QUOTE_STR),
								   new_redirection(1, OUT, "line3 $PATH line4", DOUBLE_QUOTE_STR),

							   })},
				{})}},
		ParserTestParams{
		{
		new_token("cat", WORD),
		new_token("<<", HERE_DOC),
		new_token("line1\n$PATH\nline2", DOUBLE_QUOTE),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("cat", WORD), {},
								   {new_redirection(0, HERED, "line1\n$PATH\nline2", DOUBLE_QUOTE_STR)})},
				{})}},
		ParserTestParams{
		{
		new_token("<<", HERE_DOC),
		new_token("line1\n$PATH\nline2", DOUBLE_QUOTE),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("<<", HERE_DOC), {},
								   {new_redirection(0, HERED, "line1\n$PATH\nline2", DOUBLE_QUOTE_STR)})},
				{})}},
		ParserTestParams{
		{
		new_token("<<", HERE_DOC),
		new_token("line1\nline2", SINGLE_QUOTE),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("<<", HERE_DOC), {},
								   {new_redirection(0, HERED, "line1\nline2", LITERAL)})},
				{})}},
		ParserTestParams{{
		new_token("cat", WORD),
		new_token("7<", REDIRECT_IN),
		new_token("LOGNAME", DOLLAR),
		new_token(NULL, END_OF_FILE),
	},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("cat", WORD),
												   {},  {new_redirection(7, IN, "LOGNAME", ENV_EXP)}), {})}},
		ParserTestParams{
		{
		new_token("4>>", REDIRECT_APPEND),
		new_token("PATH", DOLLAR),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("4>>", REDIRECT_APPEND), {},
								   {new_redirection(4, APPEND, "PATH", ENV_EXP)})},
				{})}},
		ParserTestParams{
		{
		new_token(">>", REDIRECT_APPEND),
		new_token("PATH", DOLLAR),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token(">>", REDIRECT_APPEND), {},
								   {new_redirection(1, APPEND, "PATH", ENV_EXP)})},
				{})}},
		ParserTestParams{
		{
		new_token(">>", REDIRECT_APPEND),
		new_token("test.txt", WORD),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token(">>", REDIRECT_APPEND), {},
								   {new_redirection(1, APPEND, "test.txt", LITERAL)})},
				{})}},
		ParserTestParams{{
		new_token("cat", WORD),
		new_token("<", REDIRECT_IN),
		new_token("input1", WORD),
		new_token(NULL, END_OF_FILE),
	},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("cat", WORD),
												   {},  {new_redirection(0, IN, "input1", LITERAL)}), {})}},
		ParserTestParams{
		{
		new_token("<", REDIRECT_IN),
		new_token("test.txt", WORD),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token("<", REDIRECT_IN), {},
								   {new_redirection(0, IN, "test.txt", LITERAL)})},
				{})}},
		ParserTestParams{
		{
		new_token(">", REDIRECT_OUT),
		new_token("test.txt", WORD),
		new_token(NULL, END_OF_FILE),
	},
		{new_test_script_node(
				CMD_NODE,
				{new_test_cmd_node(new_token(">", REDIRECT_OUT), {},
								   {new_redirection(1, OUT, "test.txt", LITERAL)})},
				{})}},
		ParserTestParams{{
		new_token("echo", BUILTIN),
		new_token("string1 ", DOUBLE_QUOTE),
		new_token(">", REDIRECT_OUT),
		new_token("output1", WORD),
		new_token(NULL, END_OF_FILE),
	},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("echo", BUILTIN),
												   {
												   new_argument("string1 ", DOUBLE_QUOTE_STR),
											   },  {new_redirection(1, OUT, "output1", LITERAL)}), {})}}

		)
	);
