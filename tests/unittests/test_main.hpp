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

#endif // TESTS_MAIN_H
