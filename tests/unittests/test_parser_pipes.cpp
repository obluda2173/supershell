#include "test_main.hpp"

void test_pipe_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens) {
    ASSERT_NE(nullptr, sn);
    ASSERT_EQ(sn->node_type, PIPE_NODE);
    ASSERT_EQ(sn->num_children, 2);
    ASSERT_NE(nullptr,sn->child1);
    ASSERT_EQ(CMD_NODE, sn->child1->node_type);
    ASSERT_NE(nullptr,sn->child2);
    ASSERT_EQ(CMD_NODE, sn->child2->node_type);

    compare_cmd_node(want.pipe_node_childs[0], (t_cmd_node)sn->child1->node_data.cmd_node);
    compare_cmd_node(want.pipe_node_childs[1], (t_cmd_node)sn->child2->node_data.cmd_node);

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
            {{new_token("echo", BUILTIN), new_token("file.txt", WORD),
              new_token("file2.txt", WORD), new_token("|", PIPE),
              new_token("ls", WORD), new_token(NULL, END_OF_FILE)}},
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
                                 })}
        )
    );
