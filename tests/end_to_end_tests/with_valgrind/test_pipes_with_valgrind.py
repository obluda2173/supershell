#!/usr/bin/env python3

import pytest
from conftest import (
    send_cmds_minishell,
    start_process_with_valgrind,
)
from assertions import assert_no_memory_error_valgrind, assert_no_open_fds_valgrind


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
    stderr_minishell = stderr_minishell.decode()

    assert_no_memory_error_valgrind(stderr_minishell)
    assert_no_open_fds_valgrind(stderr_minishell)
