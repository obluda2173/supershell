#ifndef TESTS_MAIN_H
#define TESTS_MAIN_H

#include <gtest/gtest.h>
#include <vector>


extern "C" {
#include "lexer.h"
#include "parser.h"
#include "libft.h"
}
t_token new_token(const char* content, token_type type);
t_dllist *create_token_dllist(std::vector<t_token> tokens);
#endif // TESTS_MAIN_H
