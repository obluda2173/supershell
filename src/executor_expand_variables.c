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
	after = expand_variables(ft_strchr(word + 1, '\'') + 1, data);
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
	after = expand_variables(ft_strchr(word + 1, '\"') + 1, data);
	prepend = handle_double_quotes(word + 1, data);
	result = ft_strjoin(prepend, after);
	free(prepend);
	free(after);
	return result;
}

char *expand_from_dollar(char *word, t_data *data) {
	char* next;
	char* after;
	char* prepend;
	char* result;
	next = ft_strchr(word + 1, '$');
	if (next)
		after = expand_variables(ft_strchr(next, '$'), data);
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
		after = expand_variables(next, data);
		*next = '\0';
		result = ft_strjoin(word, after);
	}
	free(after);
	return result;
}

char	*expand_variables(char *word, t_data *data)
{
	char	*result;

	word = ft_strdup(word);
	result = NULL;
	if (*word == '\'')
		result = expand_from_single_quote(word, data);
	if (*word == '\"')
		result = expand_from_double_quote(word, data);
	if (*word == '$')
		result =expand_from_dollar(word, data);
	if (!result)
		result = expand_from_here(word, data);
	free(word);
	return result;

}
