#include "test_main.hpp"

struct TestTokenizeParams {
	std::string line;
	std::vector<t_token> want_tokens;
};

class TestTokenizer : public::testing::TestWithParam<TestTokenizeParams>{};

TEST_P(TestTokenizer, firstTests) {
	TestTokenizeParams params = GetParam();
	std::vector<t_token> want_tokens = params.want_tokens;

	testing::internal::CaptureStdout();
	t_dllist *tokens = tokenize((char*)params.line.c_str());
	testing::internal::GetCapturedStdout();
	t_dllist *head = tokens;

	ASSERT_EQ(want_tokens.size(),ft_dllstsize(tokens));
	for (size_t i = 0; i < params.want_tokens.size(); i++) {
		auto want_token = want_tokens[i];
		t_token *got_token = (t_token*)head->content;
		EXPECT_STREQ(want_token.content, got_token->content);
		EXPECT_EQ( want_token.type, got_token->type);
		head = head->next;
	}
	ft_dllstclear(&tokens, free_token);
}

INSTANTIATE_TEST_SUITE_P(
	LexerTests, TestTokenizer,
	testing::Values(TestTokenizeParams{"", {{NULL, END_OF_FILE}}},
					// TestTokenizeParams{
					// 	"'", {
					// 		new_token("'", SINGLE_QUOTE),
					// 		new_token(NULL, END_OF_FILE),
					// 	}},
					// TestTokenizeParams{
					// 	"\"", {
					// 		new_token("\"", DOUBLE_QUOTE),
					// 		new_token(NULL, END_OF_FILE),
					// 	}},
					// TestTokenizeParams{
					// 	"cat CMake\tLists.txt", {
					// 		new_token("cat", WORD),
					// 		new_token("CMakeLists.txt", WORD),
					// 		new_token(NULL, END_OF_FILE),
					// 	}},
					TestTokenizeParams{
						"<", {
							new_token("<", REDIRECT_IN),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						">", {
							new_token(">", REDIRECT_OUT),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"|", {
							new_token("|", PIPE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"<<", {
							new_token("<<", HERE_DOC),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						">>", {
							new_token(">>", REDIRECT_APPEND),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"cd", {
							new_token("cd", BUILTIN),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"ls | wc", {
							new_token("ls", WORD),
							new_token("|", PIPE),
							new_token("wc", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"ls -a -l | wc -l", {
							new_token("ls", WORD),
							new_token("-a", WORD),
							new_token("-l", WORD),
							new_token("|", PIPE),
							new_token("wc", WORD),
							new_token("-l", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"> test.txt", {
							new_token(">", REDIRECT_OUT),
							new_token("test.txt", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo  $ PATH", {
							new_token("echo", BUILTIN),
							new_token("$", WORD),
							new_token("PATH", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo  $PATH", {
							new_token("echo", BUILTIN),
							new_token("PATH", DOLLAR),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo $?", {
							new_token("echo", BUILTIN),
							new_token("?", DOLLAR),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo dir1/dir2/*.c", {
							new_token("echo", BUILTIN),
							new_token("dir1/dir2/*.c", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo \"this is a double quoted string\"", {
							new_token("echo", BUILTIN),
							new_token("this is a double quoted string", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo \"string1 $PATH string2\"", {
							new_token("echo", BUILTIN),
							new_token("string1 $PATH string2", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo \'string1 $PATH string2\'", {
							new_token("echo", BUILTIN),
							new_token("string1 $PATH string2", SINGLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo \"string with 'nested single quotes' inside\"", {
							new_token("echo", BUILTIN),
							new_token("string with 'nested single quotes' inside", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo 'string with \"nested double quotes\" inside'", {
							new_token("echo", BUILTIN),
							new_token("string with \"nested double quotes\" inside", SINGLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"cat file.txt | grep 'pattern' >> output.txt", {
							new_token("cat", WORD),
							new_token("file.txt", WORD),
							new_token("|", PIPE),
							new_token("grep", WORD),
							new_token("pattern", SINGLE_QUOTE),
							new_token(">>", REDIRECT_APPEND),
							new_token("output.txt", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"export VAR=value", {
							new_token("export", BUILTIN),
							new_token("VAR", WORD),
							new_token("=", EQUAL_SIGN),
							new_token("value", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"export VAR=\"value with spaces\"", {
							new_token("export", BUILTIN),
							new_token("VAR", WORD),
							new_token("=", EQUAL_SIGN),
							new_token("value with spaces", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"unset VAR", {
							new_token("unset", BUILTIN),
							new_token("VAR", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"((expression))", {
							new_token("((expression))", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"ls | wc &", {
							new_token("ls", WORD),
							new_token("|", PIPE),
							new_token("wc", WORD),
							new_token("&", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"cat < input.txt > output.txt", {
							new_token("cat", WORD),
							new_token("<", REDIRECT_IN),
							new_token("input.txt", WORD),
							new_token(">", REDIRECT_OUT),
							new_token("output.txt", WORD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo -e \"line1\nline2\"", {
							new_token("echo", BUILTIN),
							new_token("-e", WORD),
							new_token("line1\nline2", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}}
		)
	);
