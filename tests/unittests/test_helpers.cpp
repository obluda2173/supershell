#include "test_main.hpp"

t_token new_token(const char* content, token_type type) {
	return (t_token){(char*)content, type};

}
