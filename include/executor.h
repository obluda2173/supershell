#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "libft.h"
#include "parser.h"
#include "minishell.h"
#include "executor_builtins.h"
#include "executor_expansions.h"

/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */
// executor.c
int execute_script(t_script_node *script_node,  t_data *data);

// executor_cmd_path.c
char	*get_path_env(t_list *envp);
char	*find_path(char *cmd, char *path_env);

// execute_cmd.c
int execute_command(t_cmd_node *cmd_node, t_data *data);

// executor_matrix_manipulation.c
void free_char_array(char **matrix);
size_t ft_size_char_array(char **matrix);
char **ft_matrix_join(char **matrix1, char **matrix2);

// executor_prepare_argv.c
char **list_to_argv(t_list *arguments, char *cmd_path);

// executor_export_3.c
t_list	**copy_ep(t_list *ep);
bool	should_be_escaped(char c);
void	print_env_var(char *key, char *value);
void	move_invalid_keys(t_list **ep, t_list **tmp_lst, 
								t_list **prev, t_list **last_node);

/* executor_helpers.c */
void	close_fds(int fds[2]);
t_env_var	*get_env_var(t_list *ep, char *key);

/* execute_pipeline.c */
int	execute_pipeline(t_script_node *sn, t_data *data);


#endif // EXECUTOR_
