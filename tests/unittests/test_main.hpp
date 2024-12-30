#ifndef TESTS_MAIN_H
#define TESTS_MAIN_H

#include <gtest/gtest.h>
#include <vector>


extern "C" {
#include "lexer.h"
#include "parser.h"
#include "libft.h"
}


typedef struct s_script_node_test {
	t_token token;
	t_node_type type;
	std::vector<std::string> arguments;
	int argument_count;
} t_test_script_node;

t_token new_token(const char* content, token_type type);
t_dllist *create_token_dllist(std::vector<t_token> tokens);

void free_script_node(void *sn);
t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<std::string> args, int argument_count);

#endif // TESTS_MAIN_H
