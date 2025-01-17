#include "test_main.hpp"

TEST(wildcardTestSuite, firstTest) {
	char **argv = NULL;
	const char *word = "*";
	char**temp = handle_wildcard(word, argv);
	free_matrix(temp);
};
