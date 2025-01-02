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
	std::vector<t_redirection> redirects;
} t_test_script_node;

t_token new_token(const char* content, token_type type);
t_dllist *create_token_dllist(std::vector<t_token> tokens);

t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<t_argument> args, int argument_count, std::vector<t_redirection> redirects);
t_argument new_argument(const char* literal, t_argument_type type);
t_redirection new_redirection(const char* literal, t_redirection_type type);

#endif // TESTS_MAIN_H
