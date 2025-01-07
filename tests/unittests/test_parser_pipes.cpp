#include "test_main.hpp"

INSTANTIATE_TEST_SUITE_P(
	ParserTestsPipes,
	ParserTestSuite,
	testing::Values(
		ParserTestParams{
			0, PIPE_TEST, {
				{
					new_token("echo", BUILTIN),
					new_token("|", PIPE),
					new_token("echo", BUILTIN),
					new_token(NULL, END_OF_FILE)
				}
			},
			{
				new_test_script_node(PIPE_NODE, {}, {},
									 {
										 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {}, {}),
										 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {}, {})
									 })}},
		ParserTestParams{
			0, PIPE_TEST, {
				{
					new_token("echo", BUILTIN),
					new_token("|", PIPE),
					new_token("ls", WORD),
					new_token(NULL, END_OF_FILE)
				}
			},
			{
				new_test_script_node(PIPE_NODE, {}, {},
									 {
										 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {}, {}),
										 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {}, {}), {}, {})
									 })}},
		ParserTestParams{
			0, PIPE_TEST, {
				{
					new_token("echo", BUILTIN),
					new_token("file.txt", WORD),
					new_token("|", PIPE),
					new_token("ls", WORD),
					new_token(NULL, END_OF_FILE)
				}
			},
			{
				new_test_script_node(PIPE_NODE, {}, {},
									 {
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("file.txt", LITERAL),}, {}), {}, {}),
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {}, {}), {}, {}),
									 })}},
		ParserTestParams{
			0, PIPE_TEST, {
				{
					new_token("echo", BUILTIN),
					new_token("file.txt", WORD),
					new_token("file2.txt", WORD),
					new_token("|", PIPE),
					new_token("ls", WORD),
					new_token(NULL, END_OF_FILE)
				}
			},
			{
				new_test_script_node(PIPE_NODE, {}, {},
									 {
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("file.txt", LITERAL), new_argument("file2.txt", LITERAL)}, {}), {}, {}),
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {}, {}), {}, {}),
									 })}},
		ParserTestParams{
			0, PIPE_TEST, {
				{
					new_token("echo", BUILTIN),
					new_token("|", PIPE),
					new_token("ls", WORD),
					new_token("-a", WORD),
					new_token(NULL, END_OF_FILE)
				}
			},
			{
				new_test_script_node(PIPE_NODE, {}, {},
									 {
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {}, {}),
									 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {}),
									 })}}
		)
	);
