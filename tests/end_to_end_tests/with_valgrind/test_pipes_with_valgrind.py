#!/usr/bin/env python3

import pytest
from conftest import send_cmds_minishell, start_process_with_valgrind


@pytest.mark.parametrize(
    "cmd",
    [
        ("echo hello | cat\n"),
        ("echo hello2 | wc -c | cat \n"),
        ("echo hello2 | wc -c | ls -a | cat \n"),
    ],
)
def test_pipe_with_valgrind(cmd):
    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell = send_cmds_minishell(minishell, cmd)

    stdout_minishell = stdout_minishell.decode().split("\n")
    stderr_minishell = stderr_minishell.decode().split("\n")

    for line in stderr_minishell:
        if "FILE DESCRIPTORS" in line:
            assert "FILE DESCRIPTORS: 3 open (3 std) at exit." in line

    # assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
