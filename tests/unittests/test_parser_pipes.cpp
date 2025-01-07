#include "test_main.hpp"

void compare_pipe_node(t_test_script_node want, t_script_node *sn) {
    ASSERT_NE(nullptr, sn);
    ASSERT_EQ(sn->node_type, PIPE_NODE);
    ASSERT_EQ(sn->num_children, 2);
    ASSERT_EQ(want.pipe_node_childs[0].type, sn->child1->node_type);
    ASSERT_NE(nullptr,sn->child2);
    ASSERT_EQ(want.pipe_node_childs[1].type, sn->child1->node_type);

    if (sn->child1->node_type == CMD_NODE)
        compare_cmd_node(want.pipe_node_childs[0], (t_cmd_node)sn->child1->node_data.cmd_node);
    if (sn->child1->node_type == PIPE_NODE)
        compare_pipe_node(want.pipe_node_childs[0], sn->child1);

    if (sn->child2->node_type == CMD_NODE)
        compare_cmd_node(want.pipe_node_childs[1], (t_cmd_node)sn->child2->node_data.cmd_node);
}

void test_pipe_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens) {
    compare_pipe_node(want, sn);
    free_script_node(sn);
    ft_dllstclear(&tokens, free_token);
}

INSTANTIATE_TEST_SUITE_P(
    ParserTestsPipes, ParserTestSuite,
    testing::Values(
        ParserTestParams{
            0,
            PIPE_TEST,
            {{new_token("echo", BUILTIN), new_token("|", PIPE),
              new_token("echo", BUILTIN), new_token(NULL, END_OF_FILE)}},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {},
                     {}),
                 new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {},
                     {})})},
        ParserTestParams{
            1,
            PIPE_TEST,
            {{new_token("echo", BUILTIN), new_token("|", PIPE),
              new_token("ls", WORD), new_token(NULL, END_OF_FILE)}},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {},
                     {}),
                 new_test_script_node(
                     CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {}, {}),
                     {}, {})})},
        ParserTestParams{
            2,
            PIPE_TEST,
            {{new_token("echo", BUILTIN), new_token("file.txt", WORD),
              new_token("|", PIPE), new_token("ls", WORD),
              new_token(NULL, END_OF_FILE)}},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("echo", BUILTIN),
                                          {
                                              new_argument("file.txt", LITERAL),
                                          },
                                          {}),
                        {}, {}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD), {}, {}), {},
                        {}),
                })},
        ParserTestParams{
            3,
            PIPE_TEST,
            {{
                new_token("echo", BUILTIN), new_token("file.txt", WORD),
                new_token("file2.txt", WORD), new_token("|", PIPE),
                new_token("ls", WORD),
                new_token(NULL, END_OF_FILE)
                }},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("echo", BUILTIN),
                                          {new_argument("file.txt", LITERAL),
                                           new_argument("file2.txt", LITERAL)},
                                          {}),
                        {}, {}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD), {}, {}), {},
                        {}),
                })},
        ParserTestParams{
            4,
            PIPE_TEST,
            {{new_token("echo", BUILTIN), new_token("|", PIPE),
              new_token("ls", WORD), new_token("-a", WORD),
              new_token(NULL, END_OF_FILE)}},

            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("echo", BUILTIN), {}, {}),
                        {}, {}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD),
                                          {new_argument("-a", LITERAL)}, {}),
                        {}, {}),
                })},
        ParserTestParams{5,
            PIPE_TEST,
            {{
                    new_token("echo", BUILTIN),
                    new_token("|", PIPE),
                    new_token("ls", WORD),
                    new_token("-a", WORD),
                    new_token(NULL, END_OF_FILE)
                }},
            new_test_script_node(PIPE_NODE, {}, {},
                                 {
                                     new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}), {}, {}),
                                     new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {}),
                                 })},
        ParserTestParams{6,
            PIPE_TEST,
            {{
                    new_token("echo", BUILTIN),
                    new_token(">", REDIRECT_OUT),
                    new_token("file.txt", WORD),
                    new_token("|", PIPE),
                    new_token("ls", WORD),
                    new_token("-a", WORD),
                    new_token(NULL, END_OF_FILE)
                }},
            new_test_script_node(PIPE_NODE, {}, {},
                                 {
                                     new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {new_redirection(1, OUT, "file.txt", LITERAL)}), {}, {}),
                                     new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {}),
                                 })},
        ParserTestParams{7,
            PIPE_TEST,
            {{
                    new_token("wc", WORD),
                        new_token("|", PIPE),
                    new_token("echo", BUILTIN),
                    new_token("|", PIPE),
                    new_token("ls", WORD),
                    new_token(NULL, END_OF_FILE)
                }},
            new_test_script_node(PIPE_NODE, {}, {},
                                 {
                                     new_test_script_node(PIPE_NODE, {}, {}, {
                                             new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("wc", WORD), {}, {}), {}, {}),
                                             new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", BUILTIN), {}, {}), {}, {})
                                         }),
                                     new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {}),
                                 })}
        )
    );
