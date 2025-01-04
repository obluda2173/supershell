#include "test_main.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>
extern "C" {
#include "lexer.h"
}

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

t_token new_token(const char* content, token_type type) {
	return (t_token){(char*)content, type};

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
							new_token("string1 ", DOUBLE_QUOTE),
							new_token("PATH", DOLLAR),
							new_token(" string2", DOUBLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo \'string1 $PATH string2\'", {
							new_token("echo", BUILTIN),
							new_token("string1 $PATH string2", SINGLE_QUOTE),
							new_token(NULL, END_OF_FILE),
						}}
					// TestTokenizeParams{
					// 	"cat <<EOF\nline1\nline2\nEOF", {
					// 		new_token("cat", WORD),
					// 		new_token("<<", HERE_DOC),
					// 		new_token("line1\nline2\n", WORD),	
					// 		new_token(NULL, END_OF_FILE),
					// 	}},
					// 	TestTokenizeParams{
					// 	"cat <<EOF\nline1\n$PATH\nline2\nEOF", {
					// 		new_token("cat", WORD),
					// 		new_token("<<", HERE_DOC),
					// 		new_token("line1\n", WORD),	
					// 		new_token("PATH", DOLLAR),	
					// 		new_token("line2\n", WORD),	
					// 		new_token(NULL, END_OF_FILE),
					// 	}}
		)
	);
