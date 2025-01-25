#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>


char *expand_variables(char *word, t_data *data) {
	if (*word == '$')
		return handle_dollar(word+1, data);
	char* head = word;
	while (*head) {
		if (*head == '\"') {
			head++;
			char* double_quote_str = malloc(sizeof(char) * ((ft_strchr(head+1, '\"') - head) + 1));
			ft_strlcpy(double_quote_str, head,(ft_strchr(head+1, '\"') - head));
			char* res = handle_double_quotes(double_quote_str, data);
			free(double_quote_str);
			return res;
		}
		head++;
	}
	return ft_strdup(word);

}
