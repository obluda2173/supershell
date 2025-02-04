#ifndef TESTS_MAIN_H
#define TESTS_MAIN_H

#include <vector>
#include <string>
#include <gtest/gtest.h>
# include <gmock/gmock.h>

extern "C" {
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include "executor.h"
#include "libft.h"
}

using ::testing::Invoke;

typedef struct s_test_cmd_node {
  t_token cmd_token;
  std::vector<t_argument> arguments;
  std::vector<t_redirection> redirects;
} t_test_cmd_node;

typedef struct s_test_script_node {
  t_node_type type;
  t_test_cmd_node cmd_node;
  t_error_node err_node;
  std::vector<struct s_test_script_node> childs;
} t_test_script_node;

typedef enum e_test {
  ERROR_TEST,
  CMD_TEST,
  PIPE_TEST,
} t_test_type;

struct ParserTestParams {
  int test_nbr;
  t_test_type test_type;
  std::vector<t_token> token_vec;
  t_test_script_node want;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

t_dllist *create_token_dllist(std::vector<t_token> tokens);

t_token new_token(const char* content, t_token_type type);
t_test_script_node new_test_script_node(t_node_type type, t_test_cmd_node cn, t_error_node en, std::vector<t_test_script_node> pipe_node_childs);
t_test_cmd_node new_test_cmd_node(t_token cmd_token, std::vector<t_argument> args, std::vector<t_redirection> redirects);
t_error_node new_error_node(const char* error);
t_argument new_argument(const char* literal, t_word_type type);
t_argument *new_argument_pointer(const char* literal, t_word_type type);
t_redirection new_redirection(int fd, t_redirection_type type, const char* word, t_word_type wt);

void test_cmd_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens);
void test_pipe_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens);
void test_error_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens);
void compare_cmd_node(t_test_script_node want, t_cmd_node got);

// executor
struct ExecutorTestsParams {
  const char* cmd;
  t_token_type type;
  int want_return;
  const char* want_stdout;
  const char* want_stderr;
};

class ExecutorTestSuite : public::testing::TestWithParam<ExecutorTestsParams>{};


t_script_node *new_script_node(char *cmd, t_token_type type);
char **get_envp();
t_list *get_ep();

#endif // TESTS_MAIN_H
