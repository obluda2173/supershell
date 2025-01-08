#include "test_main.hpp"
#include <gtest/gtest.h>

TEST(ParserTestSuite, ParserTestsLogicalOperators) {
	std::vector<t_token> token_vec = {
		new_token("wc", WORD),
		new_token("-l", WORD),
		new_token(">>", REDIRECT_APPEND),
		new_token("file.txt", WORD),
		new_token("<", REDIRECT_IN),
		new_token("var", DOLLAR),
		new_token("|", PIPE),
		new_token("echo", BUILTIN),
		new_token("|", PIPE),
		new_token("ls", WORD),
		new_token(NULL, END_OF_FILE)
	} ;
	t_dllist *tokens = create_token_dllist(token_vec);
	if (token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);


	t_script_node *sn = parse(tokens);
	if (token_vec.size() == 1 && token_vec[0].type == END_OF_FILE) {
		ASSERT_EQ(nullptr, sn);
		ft_dllstclear(&tokens, free_token);
		return;
	}

}
