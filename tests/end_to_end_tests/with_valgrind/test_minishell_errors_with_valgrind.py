#!/usr/bin/env python3

from conftest import (
    start_process_with_valgrind,
    parse_out_and_err_minishell,
    send_cmds_minishell,
    get_open_fds,
)
from assertions import (
    assert_no_memory_error_valgrind,
    assert_no_new_file_descriptors,
)

import pytest


@pytest.mark.parametrize(
    "cmd, err_msg, want_exit_status",
    [
        (['"'], "Unclosed double quote", 2),
        (['helllo " hello'], "Unclosed double quote", 2),
        (["helllo ' hello"], "Unclosed single quote", 2),
        (["hello } whats up"], "Invalid input", 2),
        (["hello &&& whats up"], "Parse error near &", 2),
        (["("], "Unclosed parenthesis", 2),
        (['echo ( "hello" '], "Unclosed parenthesis", 2),
        (['echo "hello" ) '], "Unclosed parenthesis", 2),
        (["(("], "Unclosed parenthesis", 2),
        (["(echo hello"], "Unclosed parenthesis", 2),
        (["echo )"], "Unclosed parenthesis", 2),
        (["((echo)"], "Unclosed parenthesis", 2),
        (["(ls && (echo hi)"], "Unclosed parenthesis", 2),
        ([")ls && (echo hi)("], "Unclosed parenthesis", 2),
        (['(ls -l && echo "((expression)))"'], "Unclosed parenthesis", 2),
        (['(ls -l && echo "((expression)))"'], "Unclosed parenthesis", 2),
        (
            ["random_command < tests/end_to_end_tests/test_files/input1.txt"],
            "Command not found",
            127,
        ),
    ],
)
def test_errors(cmd, err_msg, want_exit_status):
    minishell = start_process_with_valgrind("./minishell")

    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert len(stderr_minishell) != 0
    assert len(stdout_minishell) == 1
    assert err_msg in stderr_minishell
    assert want_exit_status == int(stdout_minishell[0])
