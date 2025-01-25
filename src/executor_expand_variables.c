#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>


char *expand_variables(char *word, t_data *data) {
	if (*word == '$')
		return handle_dollar(word+1, data);

	char* double_quote = ft_strchr(word, '\"');
	if (!double_quote)
		return ft_strdup(word);

	char* next_double_quote = ft_strchr(double_quote+1, '\"');
	if (!next_double_quote) {
		ft_putendl_fd("eher", STDOUT_FILENO);
	}
	char* double_quote_str = (char*)malloc(sizeof(char) * (next_double_quote - double_quote + 1));
	ft_strlcpy(double_quote_str, double_quote+1,next_double_quote - double_quote);
	char* res = handle_double_quotes(double_quote_str, data);
	free(double_quote_str);

	return res;

}
