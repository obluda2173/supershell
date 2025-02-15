#include "test_main.hpp"

t_script_node *new_script_node(char *cmd, t_token_type type) {
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	sn->node_type = CMD_NODE;
	sn->node_data.cmd_node.cmd_token = new_token(ft_strdup(cmd), type);
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
	sn->upstream = NULL;
	sn->downstream = NULL;
	return sn;
}

char **get_envp() {
	char** envp = (char**)malloc(sizeof(char*) * 2);
	envp[0] = ft_strdup((char*)("PATH=" + std::string(std::getenv("PATH"))).c_str());
	envp[1] = NULL;
	return envp;
}

