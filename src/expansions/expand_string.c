#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>

char *get_min_char(char *a, char *b) {
	if (b < a)
		return b;
	return a;
}
char	*get_next_break(char *word)
{
	char	*n_double;
	char	*n_single;
	char	*n_dollar;

	n_double = ft_strchr(word, '\"');
	n_single = ft_strchr(word, '\'');
	n_dollar = ft_strchr(word, '$');
	if (n_single && n_double && n_dollar)
		return get_min_char(get_min_char(n_double, n_single), n_dollar);
	if (n_double && n_single)
		return get_min_char(n_double, n_single);
	if (n_double && n_dollar)
		return get_min_char(n_double, n_dollar);
	if (n_single && n_dollar)
		return get_min_char(n_single, n_dollar);
	if (n_double)
		return n_double;
	if (n_single)
		return n_single;
	if (n_dollar)
		return n_dollar;
	return NULL;
}

char *expand_from_single_quote(char* word, t_data *data) {
	char* after;
	char* prepend;
	char* result;
	after = expand_string(ft_strchr(word + 1, '\'') + 1, data);
	prepend = malloc(sizeof(char) * (ft_strchr(word + 1, '\'') - word));
	ft_strlcpy(prepend, word + 1, (ft_strchr(word + 1, '\'') - word));
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return result;
}

char *expand_from_double_quote(char* word, t_data *data) {
	char* after;
	char* prepend;
	char* result;
	after = expand_string(ft_strchr(word + 1, '\"') + 1, data);
	prepend = handle_double_quotes(word + 1, data);
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return result;
}

bool is_env_var_char(char c) {
	if (ft_isalnum(c) || c == '_' )
		return true;
	return false;
}

char *advance_end_of_env_var(char* word) {
	if (!word)
		return NULL;
	if (*word == '?')
		return word+1;
	while (is_env_var_char(*word))
		word++;
	return word;
}

char *expand_from_dollar(char *word, t_data *data) {
	char* next;
	char* after;
	char* prepend;
	char* result;
	next = advance_end_of_env_var(word+1);
	if (next)
		after = expand_string(next, data);
	else
		after = ft_strdup("");
	prepend = handle_dollar(word + 1, data);
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return result;
}

char *expand_from_here(char *word, t_data *data) {
	char* next;
	char* after;
	char* result;

	after = NULL;
	next = get_next_break(word);
	if (!next)
		result = ft_strdup(word);
	else
	{
		after = expand_string(next, data);
		*next = '\0';
		result = ft_strjoin(word, after);
	}
	free(after);
	return result;
}

char	*expand_string(char *string, t_data *data)
{
	char	*result;

	string = ft_strdup(string);
	result = NULL;
	if (*string == '\'')
		result = expand_from_single_quote(string, data);
	if (*string == '\"')
		result = expand_from_double_quote(string, data);
	if (*string == '$')
		result =expand_from_dollar(string, data);
	if (!result)
		result = expand_from_here(string, data);
	free(string);
	return result;

}
