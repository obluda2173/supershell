#include "lexer.h"

t_token *new_eof_token() {
	t_token *eof = (t_token*)malloc(sizeof(t_token));
	if (!eof)
		return NULL;
	eof->content = NULL;
	eof->type = END_OF_FILE;
	return eof;
}
