#include "executor.h"
#include "libft.h"
#include "parser.h"

// int execute(t_script_node *script) {
// 	t_list* args = sn.node_data.cmd_node.arguments;
// 	while (args) {
// 		t_argument arg = *(t_argument*)args->content;
// 		ft_putstr_fd(arg.word, STDOUT_FILENO);
// 		args = args->next;
// 		if (args)
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 	}

// 	ft_putendl_fd("", STDOUT_FILENO);
// 	return 0;
// }

char **list_to_argv(t_list *list)
{
    char **argv;
    size_t count = 0;
    size_t i = 0;
    t_list *tmp = list;

    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return (NULL);

    tmp = list;
    while (tmp)
    {
        argv[i] = ft_strdup(((t_argument *)tmp->content)->word);
        if (!argv[i++])
        {
            free_matrix(argv);
            return (NULL);
        }
        tmp = tmp->next;
    }
    argv[i] = NULL;

    return (argv);
}

void free_matrix(char **matrix)
{
    size_t i = 0;

    if (!matrix)
        return;

    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

int execute_command(t_cmd_node *cmd_node, char **envp)
{
    pid_t pid;
    int status;
    char *cmd_path;

    cmd_path = find_path(cmd_node->cmd_token.content, envp);
    if (!cmd_path)
    {
        fprintf(stderr, "Command not found: %s\n", cmd_node->cmd_token.content);
        return 127; // Standard shell exit code for "command not found"
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free(cmd_path);
        return 1;
    }
    else if (pid == 0)
    {
        // In child process
        if (execve(cmd_path, cmd_node->cmd_token.arguments, envp) == -1)
        {
            perror("execve");
            free(cmd_path);
            exit(1); // Exit with an error code
        }
    }
    else
    {
        free(cmd_path);
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            fprintf(stderr, "Child process did not terminate normally\n");
            return 1;
        }
    }
    return 0;
}

int execute_script(t_script_node *script_node, char **envp)
{
	if (!script_node)
		return (0);


	if (script_node->node_type == CMD_NODE)
	{
		return execute_command(&(script_node->node_data.cmd_node), envp);
	}
	// else if (script_node->node_type == PIPE_NODE)
	// 	// Add logic for executing pipes
	// else if (script_node->node_type == AND_NODE || script_node->node_type == OR_NODE)
	// 	// Add logic for handling logical operators (&&, ||)
	// else if (script_node->node_type == ERROR_NODE)
	// {
	// 	fprintf(stderr, "minishell: syntax error: %s\n", script_node->node_data.error_node.error);
	// 	return (2); // Syntax error
	// }

	if (script_node->downstream)
		return execute_script(script_node->downstream, envp);

	return (0);
}
