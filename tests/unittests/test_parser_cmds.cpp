#include "test_main.hpp"

void compare_redirections(std::vector<t_redirection> want, t_list *got) {
    if (want.size() == 0) {
        ASSERT_EQ(got, nullptr);
        return;
    }
    ASSERT_NE(got, nullptr);
    ASSERT_EQ(want.size(), ft_lstsize(got));

    t_list* head = got;
    for (size_t j = 0; j < want.size(); j++) {
        t_redirection r = *(t_redirection*)head->content;
        EXPECT_EQ(want[j].fd, r.fd);
        EXPECT_EQ(want[j].type, r.type);
        EXPECT_STREQ(want[j].word, r.word);
        EXPECT_EQ(want[j].word_type, r.word_type);
        head = head->next;
    }
    ASSERT_EQ(NULL, head);
}

void compare_arguments(std::vector<t_argument> want, t_list* got) {
    EXPECT_EQ(want.size(), ft_lstsize(got));
    if (want.size() == 0)
        EXPECT_EQ(NULL, got);

    t_list* head = got;
    for (size_t j = 0; j < want.size(); j++) {
        t_argument *got_arg = (t_argument*)head->content;
        EXPECT_STREQ(want[j].word, got_arg->word);
        EXPECT_EQ(want[j].type, got_arg->type);
        head = head->next;
    }
    ASSERT_EQ(NULL, head);
}

void compare_cmd_node(t_test_script_node want, t_cmd_node got) {
    EXPECT_STREQ(want.cmd_node.cmd_token.content, got.cmd_token.content);
    EXPECT_EQ(want.cmd_node.cmd_token.type, got.cmd_token.type);
    compare_arguments(want.cmd_node.arguments, got.arguments);
    compare_redirections(want.cmd_node.redirects, got.redirections);
}

void test_cmd_cases(t_test_script_node want, t_script_node *sn, t_dllist* tokens) {
    ASSERT_NE(nullptr, sn);
    EXPECT_EQ(CMD_NODE, sn->node_type);
    EXPECT_EQ(0, sn->num_children);
    EXPECT_EQ(nullptr, sn->upstream);
    EXPECT_EQ(nullptr, sn->downstream);
    compare_cmd_node(want, sn->node_data.cmd_node);
    free_script_node(sn);
    ft_dllstclear(&tokens, free_token);
}

INSTANTIATE_TEST_SUITE_P(
    ParserTestsCmdsWithRedirections, ParserTestSuite,
    testing::Values(
        ParserTestParams{
            0,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("<<", HERE_DOC),
                new_token("line1\n$PATH\nline2", WORD),
                new_token("3>",
                          REDIRECT_OUT), // here I added the file descriptor 3
                // before the command
                new_token("\"line3 $PATH line4\"", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(
                        new_token("cat", WORD), {},
                        {
                            new_redirection(0, HERED, "line1\n$PATH\nline2", LITERAL),
                            new_redirection(3, OUT, "\"line3 $PATH line4\"", LITERAL),

                        })},
                {}, {})},
        ParserTestParams{
            1,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("<<", HERE_DOC),
                new_token("line1\n$PATH\nline2", WORD),
                new_token(">", REDIRECT_OUT),
                new_token("\"line3 $PATH line4\"", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(
                        new_token("cat", WORD), {},
                        {
                            new_redirection(0, HERED, "line1\n$PATH\nline2", LITERAL),
                            new_redirection(1, OUT, "\"line3 $PATH line4\"",
                                            LITERAL),

                        })},
                {}, {})},
        ParserTestParams{
            2,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("<<", HERE_DOC),
                new_token("line1\n$PATH\nline2", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token("cat", WORD), {},
                                   {new_redirection(0, HERED,
                                                    "line1\n$PATH\nline2", LITERAL)})},
                {}, {})},
        ParserTestParams{
            3,
            CMD_TEST,
            {
                new_token("<<", HERE_DOC),
                new_token("line1\n$PATH\nline2", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(0, HERED, "line1\n$PATH\nline2", LITERAL)})},
                {}, {})},
        ParserTestParams{
            4,
            CMD_TEST,
            {
                new_token("<<", HERE_DOC),
                new_token("'line1\nline2'", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(0, HERED, "'line1\nline2'", LITERAL)})},
                {}, {})} ,
        ParserTestParams{
            5,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("7<", REDIRECT_IN),
                new_token("$LOGNAME", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                new_test_cmd_node(new_token("cat", WORD), {}, {new_redirection(7, IN, "$LOGNAME", LITERAL)}),
                {}, {})},
        ParserTestParams{
            6,
            CMD_TEST,
            {
                new_token("4>>", REDIRECT_APPEND),
                new_token("$PATH", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(4, APPEND, "$PATH", LITERAL)})},
                {}, {})},
        ParserTestParams{
            7,
            CMD_TEST,
            {
                new_token(">>", REDIRECT_APPEND),
                new_token("$PATH", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(1, APPEND, "$PATH", LITERAL)})},
                {}, {})},
        ParserTestParams{
            8,
            CMD_TEST,
            {
                new_token(">>", REDIRECT_APPEND),
                new_token("test.txt", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(1, APPEND, "test.txt",
                                                    LITERAL)})},
                {}, {})},
        ParserTestParams{
            9,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("<", REDIRECT_IN),
                new_token("input1", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                new_test_cmd_node(new_token("cat", WORD), {},
                                  {new_redirection(0, IN, "input1", LITERAL)}),
                {}, {})},
        ParserTestParams{
            10,
            CMD_TEST,
            {
                new_token("<", REDIRECT_IN),
                new_token("test.txt", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(0, IN, "test.txt",
                                                    LITERAL)})},
                {}, {})},
        ParserTestParams{
            11,
            CMD_TEST,
            {
                new_token(">", REDIRECT_OUT),
                new_token("test.txt", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(new_token(NULL, NONE), {},
                                   {new_redirection(1, OUT, "test.txt",
                                                    LITERAL)})},
                {}, {})},
        ParserTestParams{
            12,
            CMD_TEST,
            {
                new_token("echo", BUILTIN),
                new_token("\"string1 \"", WORD),
                new_token(">", REDIRECT_OUT),
                new_token("output1", WORD),
                new_token(NULL, END_OF_FILE),
            },

            new_test_script_node(CMD_NODE,
                                 new_test_cmd_node(
                                     new_token("echo", BUILTIN),
                                     {
                                         new_argument("\"string1 \"", LITERAL),
                                     },  {new_redirection(1, OUT, "output1", LITERAL)}), {}, {})},
        ParserTestParams{
            13,
            CMD_TEST,
            {
                new_token("wc", WORD),
                new_token("$env_var2", WORD),
                new_token(">>", REDIRECT_APPEND),
                new_token("$env_var", WORD),
                new_token("-l", WORD),
                new_token("3<", REDIRECT_IN),
                new_token("\"line3 $PATH line4\"", WORD),
                new_token("-L", WORD),
                new_token("10>", REDIRECT_OUT),
                new_token("'text text'", WORD),

                new_token("\"an argument\"", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(
                        new_token("wc", WORD), {
                            new_argument("$env_var2", LITERAL),
                            new_argument("-l", LITERAL),
                            new_argument("-L", LITERAL),
                            new_argument("\"an argument\"", LITERAL),
                        },
                    {
                    new_redirection(1, APPEND, "$env_var", LITERAL),
                    new_redirection(3, IN, "\"line3 $PATH line4\"", LITERAL),
                    new_redirection(10, OUT, "'text text'", LITERAL),

                })},
            {}, {})},
        ParserTestParams{
            14,
            CMD_TEST,
            {
                new_token("<<", HERE_DOC),
                new_token("line1\n$PATH\nline2", WORD),
                new_token("cat", WORD),
                new_token(">", REDIRECT_OUT),
                new_token("\"line3 $PATH line4\"", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
                CMD_NODE,
                {new_test_cmd_node(
                        new_token("cat", WORD), {},
                        {
                            new_redirection(0, HERED, "line1\n$PATH\nline2", LITERAL),
                            new_redirection(1, OUT, "\"line3 $PATH line4\"",
                                            LITERAL),

                        })},
                {}, {})}
    )
);


INSTANTIATE_TEST_SUITE_P(
    ParserTestsCmdsWithoutRedirections, ParserTestSuite,
    testing::Values(
        ParserTestParams{0, CMD_TEST, {new_token(NULL, END_OF_FILE)}, {}},
        ParserTestParams{
        1,
        CMD_TEST,
        {
        new_token("echo", BUILTIN),
        new_token(NULL, END_OF_FILE),
    },
        new_test_script_node(
            CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}),
            {}, {})},
        ParserTestParams{
        2,
        CMD_TEST,
        {
        new_token("echo", BUILTIN),
        new_token("file.txt", WORD),
        new_token(NULL, END_OF_FILE),
    },
        new_test_script_node(CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {
                                               new_argument("file.txt", LITERAL),
                                           },
                                               {}),
                             {}, {})},
        ParserTestParams{3, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("file1.txt", WORD),
                         new_token("file2.txt", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {new_argument("file1.txt", LITERAL),
                                                new_argument("file2.txt", LITERAL)},
                                               {}),
                             {}, {})},
        ParserTestParams{4, CMD_TEST,
                         {
                         new_token("wc", WORD),
                         new_token("-l", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("wc", WORD),
                                               {
                                               new_argument("-l", LITERAL),
                                           },
                                               {}),
                             {}, {})},
        ParserTestParams{5, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("$PATH", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(CMD_NODE,
                                              new_test_cmd_node(new_token("echo", BUILTIN),
                                                                {
                                                                new_argument("$PATH", LITERAL),
                                                            },
                                                                {}),
                                              {}, {})},
        ParserTestParams{6, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("$?", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {new_argument("$?", LITERAL),},
                                               {}),
                             {}, {})},
        ParserTestParams{7, CMD_TEST,{
                new_token("echo", BUILTIN),
                new_token("$", WORD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(
            CMD_NODE,
            new_test_cmd_node(new_token("echo", BUILTIN),
                              {
                              new_argument("$", LITERAL),
                          },
                              {}),
            {}, {})},
        ParserTestParams{8, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("some_*_file.c", WILDCARD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {
                                               new_argument("some_*_file.c",
                                                            WILDCARD_EXP),
                                           },
                                               {}),
                             {}, {})},
        ParserTestParams{9, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("\"this is a double quoted string\"", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(
                                 new_token("echo", BUILTIN),
                                 {
                                 new_argument("\"this is a double quoted string\"", LITERAL),
                             },
                                 {}),
                             {}, {})},
        ParserTestParams{10, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("'string1 $PATH string2'", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {
                                               new_argument("'string1 $PATH string2'", LITERAL),
                                           },
                                               {}),
                             {}, {})},
        ParserTestParams{11, CMD_TEST,
                         {
                         new_token("echo", BUILTIN), new_token("\"string1 $PATH string2\"", WORD),
                         new_token("\"string1 \"", WORD), new_token("$PATH", WORD),    new_token("\" string2\"", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {
                                               new_argument("\"string1 $PATH string2\"", LITERAL),
                                               new_argument("\"string1 \"", LITERAL),
                                               new_argument("$PATH", LITERAL),
                                               new_argument("\" string2\"", LITERAL),
                                           },
                                               {}),
                             {}, {})},
        ParserTestParams{
            12,
            CMD_TEST,
            {
                new_token("cat", WORD),
                new_token("<", REDIRECT_IN),
                new_token("*.txt", WILDCARD),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(CMD_NODE, {new_test_cmd_node(new_token("cat", WORD), {}, {new_redirection(0, IN, "*.txt", WILDCARD_EXP)})}, {}, {})},
        ParserTestParams{
            13,
            CMD_TEST,
            {
                new_token("which", WORD),
                new_token("echo", BUILTIN),
                new_token(NULL, END_OF_FILE),
            },
            new_test_script_node(CMD_NODE, {new_test_cmd_node(new_token("which", WORD), {new_argument("echo", LITERAL)}, {})}, {}, {})},
        ParserTestParams{14, CMD_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("*", WILDCARD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN),
                                               {new_argument("*", WILDCARD_EXP),
                                           },
                                               {}),
                             {}, {})}
    ));

INSTANTIATE_TEST_SUITE_P(
    ParserTestsCmdsWithParanthesis, ParserTestSuite,
    testing::Values(
        ParserTestParams{0, CMD_TEST,
                         {
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("hello", WORD),
                         new_token(")", RPAREN),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             CMD_NODE,
                             new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("hello", LITERAL),}, {}), {}, {})},
        ParserTestParams{1, PIPE_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("string1", WORD),
                         new_token("&&", AND),
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("string2", WORD),
                         new_token("&&", AND),
                         new_token("echo", BUILTIN),
                         new_token("string3", WORD),
                         new_token(")", RPAREN),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             AND_NODE, {}, {}, {
                                 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string1", LITERAL)} , {}), {}, {}),
                                 new_test_script_node(AND_NODE, {}, {}, {
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string2", LITERAL)} , {}), {}, {}),
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string3", LITERAL)} , {}), {}, {})
                                     }),
                             })},
        ParserTestParams{2, PIPE_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("string1", WORD),
                         new_token("&&", AND),
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("string2", WORD),
                         new_token(")", RPAREN),
                         new_token("|", PIPE),
                         new_token("echo", BUILTIN),
                         new_token("string3", WORD),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             AND_NODE, {}, {}, {
                                 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string1", LITERAL)} , {}), {}, {}),
                                 new_test_script_node(PIPE_NODE, {}, {}, {
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string2", LITERAL)} , {}), {}, {}),
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string3", LITERAL)} , {}), {}, {})
                                     }),
                             })},
        ParserTestParams{3, PIPE_TEST,
                         {
                         new_token("echo", BUILTIN),
                         new_token("string1", WORD),
                         new_token("&&", AND),
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("string2", WORD),
                         new_token("|", PIPE),
                         new_token("echo", BUILTIN),
                         new_token("string3", WORD),
                         new_token(")", RPAREN),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             AND_NODE, {}, {}, {
                                 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string1", LITERAL)} , {}), {}, {}),
                                 new_test_script_node(PIPE_NODE, {}, {}, {
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string2", LITERAL)} , {}), {}, {}),
                                         new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("string3", LITERAL)} , {}), {}, {})
                                     }),
                             })},
        ParserTestParams{1, PIPE_TEST,
                         {
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("hello", WORD),
                         new_token(")", RPAREN),
                         new_token("|", PIPE),
                         new_token("(", LPAREN),
                         new_token("echo", BUILTIN),
                         new_token("world", WORD),
                         new_token(")", RPAREN),
                         new_token(NULL, END_OF_FILE),
                     },
                         new_test_script_node(
                             PIPE_NODE, {}, {}, {
                                 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("hello", LITERAL)} , {}), {}, {}),
                                 new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("world", LITERAL)} , {}), {}, {})
                             })}
    ));
