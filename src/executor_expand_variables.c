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
		word++;
		char* res = expand_variables(ft_strchr(word, '\"') + 1, data);
		char* double_quote_expanded = handle_double_quotes(word, data);
		char* new_res = ft_strjoin(double_quote_expanded, res);
		free(res);
		free(double_quote_expanded);
		return new_res;
	}
	if (*word == '\'') {
		word++;
		char* res = expand_variables(ft_strchr(word, '\'')+1, data);

		char* single_quote_string = malloc(sizeof(char) * (ft_strchr(word,  '\'') - word + 1));
		ft_strlcpy(single_quote_string, word, (ft_strchr(word,  '\'') - word + 1));
		char* new_res = ft_strjoin(single_quote_string, res);
		free(res);
		free(single_quote_string);
		return new_res;
	}

	word = ft_strdup(word);
	char* next = ft_strchr(word, '\"');
	if (next != NULL) {
		char* res = expand_variables(next, data);
		*next = '\0';
		char* new_res = ft_strjoin(word, res);
		free(res);
		free(word);
		return new_res;
	}

	next = ft_strchr(word, '\'');
	if (next != NULL) {
		char* res = expand_variables(next, data);
		*next = '\0';
		char* new_res = ft_strjoin(word, res);
		free(res);
		free(word);
		return new_res;
	}
	return word;
}
