#include "test_main.hpp"

INSTANTIATE_TEST_SUITE_P(
	ParserTestsCmdsWithoutRedirections, ParserTestSuite,
	testing::Values(
		ParserTestParams{{new_token(NULL, END_OF_FILE)}, {}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}),
					{})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{
														new_argument("file.txt",
																	 LITERAL),
													},
													{}),
								  {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file1.txt", WORD),
				new_token("file2.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {new_argument("file1.txt", LITERAL),
									   new_argument("file2.txt", LITERAL)},
									  {}),
					{})}},
		ParserTestParams{{
				new_token("wc", WORD),
				new_token("-l", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("wc", WORD),
												   {
													   new_argument("-l", LITERAL),
												   },
												   {}),
								 {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("PATH", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{
														new_argument("PATH",
																	 ENV_EXP),
													},
													{}),
								  {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("?", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {
										  new_argument("?", EXIT_STATUS_EXP),
									  },
									  {}),
					{})}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("echo", BUILTIN),
												   {
													   new_argument("", ENV_EXP),
												   },
												   {}),
								 {})}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("some_*_file.c", WILDCARD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("echo", BUILTIN),
												   {
													   new_argument("some_*_file.c",
																	WILDCARD_EXP),
												   },
												   {}),
								 {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("this is a double quoted string", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(
						new_token("echo", BUILTIN),
						{
							new_argument("this is a double quoted string", DOUBLE_QUOTE_STR),
						},
						{}),
					{})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("string1 $PATH string2", SINGLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {
										  new_argument("string1 $PATH string2",
													   LITERAL),
									  },
									  {}),
					{})}},
		ParserTestParams{{
				new_token("echo", BUILTIN), new_token("string1 $PATH string2", DOUBLE_QUOTE),
				new_token("string1 ", DOUBLE_QUOTE), new_token("PATH", DOLLAR),    new_token(" string2", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
						 {
							 new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("echo", BUILTIN),
												   {
													   new_argument("string1 $PATH string2", DOUBLE_QUOTE_STR),
													   new_argument("string1 ", DOUBLE_QUOTE_STR),
													   new_argument("PATH", ENV_EXP),
													   new_argument(" string2", DOUBLE_QUOTE_STR),
												   },
												   {}),
								 {})}}
		));
