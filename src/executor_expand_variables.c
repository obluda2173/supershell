#include "libft.h"
#include "parser.h"
#include "lexer.h"
#include "minishell.h"
#include "executor.h"
#include <unistd.h>


char *expand_variables(char *word, t_data *data) {
	if (*word == '\0')
		return ft_strdup("");

	if (*word == '$')
		return handle_dollar(word+1, data);

	if (*word == '\"') {
		char* next_double_quote = ft_strchr(word+1, '\"');
		char* res = expand_variables(next_double_quote+1, data);
		char* double_quote_expanded = handle_double_quotes(word+1, data);
		char* new_res = ft_strjoin(double_quote_expanded, res);
		free(res);
		free(double_quote_expanded);
		return new_res;
	}

	char* next = ft_strchr(word, '\"');
	if (next != NULL) {
		char* res = expand_variables(next, data);
		char* until_double_quote = malloc(sizeof(char) * (next - word + 1));
		ft_strlcpy(until_double_quote, word, next-word+1);
		char* new_res = ft_strjoin(until_double_quote, res);
		free(res);
		free(until_double_quote);
		return new_res;
	}
	return ft_strdup(word);
}
