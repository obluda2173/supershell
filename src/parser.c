#include "parser.h"
#include "libft.h"

t_token copy_token(t_token token) {
	t_token copy;
	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return copy;
}


t_list *parse(t_dllist *tokens) {
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	if (!sn)
		return NULL;
	sn->token = copy_token(*(t_token*)tokens->content);
	t_list *script = ft_lstnew(sn);
	return script;
}
