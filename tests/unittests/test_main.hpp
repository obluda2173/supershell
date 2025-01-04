#ifndef TESTS_MAIN_H
#define TESTS_MAIN_H

#include <vector>
#include <string>
#include <gtest/gtest.h>

extern "C" {
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "libft.h"
}

typedef struct s_test_cmd_node {
	t_token cmd_token;
	std::vector<t_argument> arguments;
	std::vector<t_redirection> redirects;
} t_test_cmd_node;

typedef struct s_test_script_node {
	t_node_type type;
	t_test_cmd_node cmd_node;
	t_error_node err_node;
} t_test_script_node;


t_token new_token(const char* content, token_type type);
t_dllist *create_token_dllist(std::vector<t_token> tokens);

t_test_cmd_node new_test_cmd_node(t_token cmd_token, std::vector<t_argument> args, std::vector<t_redirection> redirects);
t_error_node new_error_node(const char* error);
t_test_script_node new_test_script_node(t_node_type type, t_test_cmd_node cn, t_error_node en);
t_argument new_argument(const char* literal, t_word_type type);
t_redirection new_redirection(const char* literal, t_redirection_type type, const char* word, t_word_type wt);
void compare_cmd_node(t_test_script_node want, t_cmd_node got);

#endif // TESTS_MAIN_H
