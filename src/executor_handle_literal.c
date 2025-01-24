#include "executor.h"
#include "libft.h"


char *handle_literal(char *word, t_data *data) {
	if (word[0] == '$')
		return handle_dollar(word+1, data);
	if (word[0] == '\"')
		return handle_double_quotes(word, data);
	return ft_strdup(word);
}
