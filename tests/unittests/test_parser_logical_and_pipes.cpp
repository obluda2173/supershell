#include "test_main.hpp"

void compare_logical_or_pipe_node(t_test_script_node want, t_script_node *sn) {
    ASSERT_NE(nullptr, sn);
    ASSERT_EQ(want.type, sn->node_type);
    ASSERT_EQ(sn->num_children, 2);
    ASSERT_NE(nullptr,sn->upstream);
    ASSERT_EQ(want.childs[0].type, sn->upstream->node_type);
    ASSERT_NE(nullptr,sn->downstream);
    ASSERT_EQ(want.childs[1].type, sn->downstream->node_type);

    if (sn->upstream->node_type == CMD_NODE)
        compare_cmd_node(want.childs[0], (t_cmd_node)sn->upstream->node_data.cmd_node);
    if (sn->upstream->node_type == PIPE_NODE || sn->upstream->node_type == AND_NODE)
        compare_logical_or_pipe_node(want.childs[0], sn->upstream);

    if (sn->downstream->node_type == CMD_NODE)
        compare_cmd_node(want.childs[1], (t_cmd_node)sn->downstream->node_data.cmd_node);
    if (sn->downstream->node_type == PIPE_NODE || sn->downstream->node_type == AND_NODE)
        compare_logical_or_pipe_node(want.childs[1], sn->downstream);
}

void test_pipe_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens) {
    compare_logical_or_pipe_node(want, sn);
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
        ParserTestParams{
            5,
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
        ParserTestParams{
            6,
            PIPE_TEST,
            {{new_token("echo", BUILTIN), new_token(">", REDIRECT_OUT),
              new_token("file.txt", WORD), new_token("|", PIPE),
              new_token("ls", WORD), new_token("-a", WORD),
              new_token(NULL, END_OF_FILE)}},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("echo", BUILTIN), {},
                                          {new_redirection(1, OUT, "file.txt",
                                                           LITERAL)}),
                        {}, {}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD),
                                          {new_argument("-a", LITERAL)}, {}),
                        {}, {}),
                })},
        ParserTestParams{
            7,
            PIPE_TEST,
            {{new_token("wc", WORD), new_token("|", PIPE),
              new_token("echo", BUILTIN), new_token("|", PIPE),
              new_token("ls", WORD), new_token(NULL, END_OF_FILE)}},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        PIPE_NODE, {}, {},
                        {new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("wc", WORD), {}, {}),
                             {}, {}),
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN), {},
                                               {}),
                             {}, {})}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD), {}, {}), {},
                        {}),
                })},
        ParserTestParams{
            8,
            PIPE_TEST,
            {new_token("wc", WORD), new_token("-l", WORD),
              new_token(">>", REDIRECT_APPEND), new_token("file.txt", WORD),
              new_token("<", REDIRECT_IN), new_token("var", DOLLAR),
              new_token("|", PIPE), new_token("echo", BUILTIN),
              new_token("|", PIPE), new_token("ls", WORD),
              new_token(NULL, END_OF_FILE)},
            new_test_script_node(
                PIPE_NODE, {}, {},
                {
                    new_test_script_node(
                        PIPE_NODE, {}, {},
                        {new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(
                                 new_token("wc", WORD),
                                 {new_argument("-l", LITERAL)},
                                 {new_redirection(1, APPEND, "file.txt",
                                                  LITERAL),
                                  new_redirection(0, IN, "var", ENV_EXP)}),
                             {}, {}),
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN), {},
                                               {}),
                             {}, {})}),
                    new_test_script_node(
                        CMD_NODE,
                        new_test_cmd_node(new_token("ls", WORD), {}, {}), {},
                        {}),
                })}
        )
    );

INSTANTIATE_TEST_SUITE_P(ParserTestsLocical, ParserTestSuite,
                         testing::Values(ParserTestParams{
                             0,
                             PIPE_TEST,
                             {
                                 new_token("echo", BUILTIN),
                                 new_token("hello", WORD),
                                 new_token("&&", AND),
                                 new_token("ls", WORD),
                                 new_token("-a", WORD),
             new_token(NULL, END_OF_FILE)},
            new_test_script_node(
                AND_NODE, {}, {},
                {new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("echo", BUILTIN),
                                       {new_argument("hello", LITERAL)}, {}),
                     {}, {}),
                 new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("ls", WORD),
                                       {new_argument("-a", LITERAL)}, {}),
                     {}, {})})},
        ParserTestParams{
            1,
            PIPE_TEST,
            {new_token("echo", BUILTIN), new_token("hello", WORD),
             new_token("&&", AND), new_token("wc", WORD), new_token("-l", WORD),
             new_token("&&", AND), new_token("ls", WORD), new_token("-a", WORD),
             new_token(NULL, END_OF_FILE)},
            new_test_script_node(
                AND_NODE, {}, {},
                {new_test_script_node(
                     AND_NODE, {}, {},
                     {
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {new_argument("hello", LITERAL)},
                                               {}),
                             {}, {}),
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("wc", WORD),
                                               {new_argument("-l", LITERAL)},
                                               {}),
                             {}, {}),
                     }),
                 new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("ls", WORD),
                                       {new_argument("-a", LITERAL)}, {}),
                     {}, {})})},
        ParserTestParams{
            2,
            PIPE_TEST,
            {new_token("echo", BUILTIN), new_token("hello", WORD),
             new_token("||", OR), new_token("wc", WORD), new_token("-l", WORD),
             new_token("||", OR), new_token("ls", WORD), new_token("-a", WORD),
             new_token(NULL, END_OF_FILE)},
            new_test_script_node(
                OR_NODE, {}, {},
                {new_test_script_node(
                     OR_NODE, {}, {},
                     {
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {new_argument("hello", LITERAL)},
                                               {}),
                             {}, {}),
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("wc", WORD),
                                               {new_argument("-l", LITERAL)},
                                               {}),
                             {}, {}),
                     }),
                 new_test_script_node(
                     CMD_NODE,
                     new_test_cmd_node(new_token("ls", WORD),
                                       {new_argument("-a", LITERAL)}, {}),
                     {}, {})})},
        ParserTestParams{3,
                         PIPE_TEST,
                         {
                             new_token("echo", BUILTIN),
                             new_token("hello", WORD),
                          new_token("||", OR),
                          new_token("wc", WORD),
                          new_token("-l", WORD),
                          new_token("||", OR),
                          new_token("ls", WORD),
                          new_token("-a", WORD),
                          new_token("|", PIPE),
                          new_token("cat", WORD),
                          new_token("-b", WORD),
                          new_token(NULL, END_OF_FILE)},
                         new_test_script_node(
                             OR_NODE, {}, {},
                             {
                                 new_test_script_node(OR_NODE, {}, {}, {
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("hello", LITERAL)}, {}), {}, {}),
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("wc", WORD), {new_argument("-l", LITERAL)}, {}), {}, {}),
                                     }),
                                 new_test_script_node(PIPE_NODE, {}, {}, {
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {}),
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("cat", WORD), {new_argument("-b", LITERAL)}, {}), {}, {}),
                                     })
                             })}
        )
    );
