#ifndef TESTS_MAIN_H
#define TESTS_MAIN_H

#include <gtest/gtest.h>
#include <vector>


extern "C" {
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "libft.h"
}

typedef struct s_test_script_node {
	t_token token;
	t_node_type type;
	std::vector<t_argument> arguments;
	int argument_count;
} t_test_script_node;

t_token new_token(const char* content, token_type type);
t_dllist *create_token_dllist(std::vector<t_token> tokens);

t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<t_argument> args, int argument_count);
t_argument new_argument(const char* literal);

#endif // TESTS_MAIN_H
