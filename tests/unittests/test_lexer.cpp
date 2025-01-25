#include "test_main.hpp"

struct TestTokenizeParams {
	std::string line;
	std::vector<t_token> want_tokens;
};

class TestTokenizer : public::testing::TestWithParam<TestTokenizeParams>{};

TEST_P(TestTokenizer, firstTests) {
	TestTokenizeParams params = GetParam();
	std::vector<t_token> want_tokens = params.want_tokens;

	// testing::internal::CaptureStdout();
	t_dllist *tokens = tokenize((char*)params.line.c_str());
	// testing::internal::GetCapturedStdout();
	t_dllist *head = tokens;

	EXPECT_EQ(want_tokens.size(),ft_dllstsize(tokens));
	for (size_t i = 0; i < params.want_tokens.size(); i++) {
		auto want_token = want_tokens[i];
		t_token *got_token = (t_token*)head->content;
		std::cout << "content: " << got_token->content << ", type: " << got_token->type << "\n";
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
					TestTokenizeParams{">>",
									   {
										   new_token(">>", REDIRECT_APPEND),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"cd",
									   {
										   new_token("cd", BUILTIN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"ls | wc",
									   {
										   new_token("ls", WORD),
										   new_token("|", PIPE),
										   new_token("wc", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"ls -a -l | wc -l",
									   {
										   new_token("ls", WORD),
										   new_token("-a", WORD),
										   new_token("-l", WORD),
										   new_token("|", PIPE),
										   new_token("wc", WORD),
										   new_token("-l", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"> test.txt",
									   {
										   new_token(">", REDIRECT_OUT),
										   new_token("test.txt", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo  $ PATH",
									   {
										   new_token("echo", BUILTIN),
										   new_token("$", WORD),
										   new_token("PATH", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo  $PATH",
									   {
										   new_token("echo", BUILTIN),
										   new_token("$PATH", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo $?",
									   {
										   new_token("echo", BUILTIN),
										   new_token("$?", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo dir1/dir2/*.c",
									   {
										   new_token("echo", BUILTIN),
										   new_token("dir1/dir2/*.c", WILDCARD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo *$PATH",
									   {
										   new_token("echo", BUILTIN),
										   new_token("*$PATH", WILDCARD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{
									"echo \"this is a double quoted string\"",
									{
										new_token("echo", BUILTIN),
										new_token("\"this is a double quoted string\"", WORD),
										new_token(NULL, END_OF_FILE),
									}},
					TestTokenizeParams{"echo \"string1 $PATH string2\"",
									   {
										   new_token("echo", BUILTIN),
										   new_token("\"string1 $PATH string2\"", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"echo \'string1 $PATH string2\'",
									   {
										   new_token("echo", BUILTIN),
										   new_token("\'string1 $PATH string2\'", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{
										"echo \"string with \'nested single quotes\' inside\"",
										{
											new_token("echo", BUILTIN),
											new_token("\"string with \'nested single quotes\' inside\"", WORD),
											new_token(NULL, END_OF_FILE),
										}},
									TestTokenizeParams{
										"echo 'string with \"nested double quotes\" inside'",
										{
											new_token("echo", BUILTIN),
											new_token("\'string with \"nested double quotes\" inside\'", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"cat file.txt | grep 'pattern' >> output.txt",
										{
											new_token("cat", WORD),
											new_token("file.txt", WORD),
											new_token("|", PIPE),
											new_token("grep", WORD),
											new_token("\'pattern\'", WORD),
											new_token(">>", REDIRECT_APPEND),
											new_token("output.txt", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"export VAR=value",
										{
											new_token("export", BUILTIN),
											new_token("VAR=value", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"export VAR=\"value with spaces\"",
										{
											new_token("export", BUILTIN),
											new_token("VAR=\"value with spaces\"", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"unset VAR",
										{
											new_token("unset", BUILTIN),
											new_token("VAR", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"ls | wc &",
										{
											new_token("ls", WORD),
											new_token("|", PIPE),
											new_token("wc", WORD),
											new_token("&", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"cat < input.txt > output.txt",
										{
											new_token("cat", WORD),
											new_token("<", REDIRECT_IN),
											new_token("input.txt", WORD),
											new_token(">", REDIRECT_OUT),
											new_token("output.txt", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"echo -e \"line1\nline2\"",
										{
											new_token("echo", BUILTIN),
											new_token("-e", WORD),
											new_token("\"line1\nline2\"", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"ls -l ",
										{
											new_token("ls", WORD),
											new_token("-l", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{" ls -l ",
										{
											new_token("ls", WORD),
											new_token("-l", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"  ls -l ",
										{
											new_token("ls", WORD),
											new_token("-l", WORD),
											new_token(NULL, END_OF_FILE),
										}},
					TestTokenizeParams{"  ls -l  ",
									   {
										   new_token("ls", WORD),
										   new_token("-l", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"  cat  cat  ls  ",
									   {
										   new_token("cat", WORD),
										   new_token("cat", WORD),
										   new_token("ls", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"cat  file.txt ",
									   {
										   new_token("cat", WORD),
										   new_token("file.txt", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{" echo  hello world  ",
									   {
										   new_token("echo", BUILTIN),
										   new_token("hello", WORD),
										   new_token("world", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"  grep -r test  ",
									   {
										   new_token("grep", WORD),
										   new_token("-r", WORD),
										   new_token("test", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   touch file1.txt  ",
									   {
										   new_token("touch", WORD),
										   new_token("file1.txt", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"  ls -a -l  ",
									   {
										   new_token("ls", WORD),
										   new_token("-a", WORD),
										   new_token("-l", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   mv file1.txt file2.txt ",
									   {
										   new_token("mv", WORD),
										   new_token("file1.txt", WORD),
										   new_token("file2.txt", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   rm -rf folder ",
									   {
										   new_token("rm", WORD),
										   new_token("-rf", WORD),
										   new_token("folder", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   echo \"hello world\" ",
									   {
										   new_token("echo", BUILTIN),
										   new_token("\"hello world\"", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"  cat -n file1.txt ",
									   {
										   new_token("cat", WORD),
										   new_token("-n", WORD),
										   new_token("file1.txt", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   ls -l -a -h ",
									   {
										   new_token("ls", WORD),
										   new_token("-l", WORD),
										   new_token("-a", WORD),
										   new_token("-h", WORD),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"   (ls -l) ",
									   {
										   new_token("(", LPAREN),
										   new_token("ls", WORD),
										   new_token("-l", WORD),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(echo hello)",
									   {
										   new_token("(", LPAREN),
										   new_token("echo", BUILTIN),
										   new_token("hello", WORD),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"  (cd ..)",
									   {
										   new_token("(", LPAREN),
										   new_token("cd", BUILTIN),
										   new_token("..", WORD),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(((ls)))",
									   {
										   new_token("(", LPAREN),
										   new_token("(", LPAREN),
										   new_token("(", LPAREN),
										   new_token("ls", WORD),
										   new_token(")", RPAREN),
										   new_token(")", RPAREN),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(   ls   -a   )",
									   {
										   new_token("(", LPAREN),
										   new_token("ls", WORD),
										   new_token("-a", WORD),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(mkdir new_dir) && (cd new_dir)",
									   {
										   new_token("(", LPAREN),
										   new_token("mkdir", WORD),
										   new_token("new_dir", WORD),
										   new_token(")", RPAREN),
										   new_token("&&", AND),
										   new_token("(", LPAREN),
										   new_token("cd", BUILTIN),
										   new_token("new_dir", WORD),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(echo (nested))",
									   {
										   new_token("(", LPAREN),
										   new_token("echo", BUILTIN),
										   new_token("(", LPAREN),
										   new_token("nested", WORD),
										   new_token(")", RPAREN),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"((echo test) && (ls))",
									   {
										   new_token("(", LPAREN),
										   new_token("(", LPAREN),
										   new_token("echo", BUILTIN),
										   new_token("test", WORD),
										   new_token(")", RPAREN),
										   new_token("&&", AND),
										   new_token("(", LPAREN),
										   new_token("ls", WORD),
										   new_token(")", RPAREN),
										   new_token(")", RPAREN),
										   new_token(NULL, END_OF_FILE),
									   }},
					TestTokenizeParams{"(ls echo done)",
						{
							new_token("(", LPAREN),
								new_token("ls", WORD),
								new_token("echo", BUILTIN),
								new_token("done", WORD),
								new_token(")", RPAREN),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"(ls -l) || (echo \'Error\')",
						{
							new_token("(", LPAREN),
								new_token("ls", WORD),
								new_token("-l", WORD),
								new_token(")", RPAREN),
								new_token("||", OR),
								new_token("(", LPAREN),
								new_token("echo", BUILTIN),
								new_token("\'Error\'", WORD),
								new_token(")", RPAREN),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"(((echo) && (echo)) || echo)",
						{
							new_token("(", LPAREN),
								new_token("(", LPAREN),
								new_token("(", LPAREN),
								new_token("echo", BUILTIN),
								new_token(")", RPAREN),
								new_token("&&", AND),
								new_token("(", LPAREN),
								new_token("echo", BUILTIN),
								new_token(")", RPAREN),
								new_token(")", RPAREN),
								new_token("||", OR),
								new_token("echo", BUILTIN),
								new_token(")", RPAREN),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"echo <<EOF > file.txt",
						{
							new_token("echo", BUILTIN),
								new_token("<<", HERE_DOC),
								new_token("EOF", WORD),
								new_token(">", REDIRECT_OUT),
								new_token("file.txt", WORD),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"echo << EOF > file.txt",
						{
							new_token("echo", BUILTIN),
								new_token("<<", HERE_DOC),
								new_token("EOF", WORD),
								new_token(">", REDIRECT_OUT),
								new_token("file.txt", WORD),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"echo << 'EOF' > file.txt",
						{
							new_token("echo", BUILTIN),
								new_token("<<", HERE_DOC),
								new_token("\'EOF\'", WORD),
								new_token(">", REDIRECT_OUT),
								new_token("file.txt", WORD),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"<<EOF <<EOF1",
						{
							new_token("<<", HERE_DOC),
								new_token("EOF", WORD),
								new_token("<<", HERE_DOC),
								new_token("EOF1", WORD),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{"echo any\"something\"",
						{
							new_token("echo", BUILTIN),
								new_token("any\"something\"", WORD),
								new_token(NULL, END_OF_FILE),
								}},
					TestTokenizeParams{
						"echo *$VAR", {
							new_token("echo", BUILTIN),
							new_token("*$VAR", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo $LOGNAME*\"some string\"\'another string\'", {
							new_token("echo", BUILTIN),
							new_token("$LOGNAME*\"some string\"\'another string\'", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}},
	// 	the following tests work just fine and should stay
					TestTokenizeParams{
						"echo *src", {
							new_token("echo", BUILTIN),
							new_token("*src", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo src*", {
							new_token("echo", BUILTIN),
							new_token("src*", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"echo *src*", {
							new_token("echo", BUILTIN),
							new_token("*src*", WILDCARD),
							new_token(NULL, END_OF_FILE),
						}}
					// TestTokenizeParams{
					// 	"export asdf$PATH", {
					// 		new_token("export", BUILTIN),
					// 		new_token("asdf$PATH", WORD),
					// 		new_token(NULL, END_OF_FILE),
					// 	}},
					// TestTokenizeParams{
					// 	"export VAR1=\"$PATH\"", {
					// 		new_token("echo", BUILTIN),
					// 		new_token("VAR1=$PATH", WORD),
					// 		new_token(NULL, END_OF_FILE),
					// 			}},
					// TestTokenizeParams{
					// 	"export VAR1='$PATH'", {
					// 		new_token("echo", BUILTIN),
					// 			new_token("VAR1=$PATH", WORD),
					// 			new_token(NULL, END_OF_FILE),
					// 			}}
					// TestTokenizeParams{
					// 	"export VAR1=\"var1\" VAR2=\"var2\" VAR3= VAR4", {
					// 		new_token("export", BUILTIN),
					// 		new_token("VAR1=\"var1\"", WORD),
					// 		new_token("VAR2=\"var2\"", WORD),
					// 		new_token("VAR3=", WORD),
					// 		new_token("VAR4", WORD),
					// 		new_token(NULL, END_OF_FILE),
					// 	}}
		)
	);
