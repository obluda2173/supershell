#!/usr/bin/env python3
import pytest

from assertions import (
    assert_no_memory_error_fsanitize,
    assert_no_new_file_descriptors,
    assert_same_lines_ordered,
)
from conftest import (
    get_open_fds,
    get_prompt_minishell,
    parse_out_and_err_minishell,
    send_cmds_minishell_with_open_fds,
    start_process,
)


@pytest.mark.parametrize(
    "cmd",
    [
        (["echo *"]),
        (["echo .*"]),
        (["echo *s"]),
        (["echo c*"]),
        (["echo *.c"]),
        (["echo ./*"]),
        # the next ones should be handled extra, they should be printed out as is
        # (["echo ./*"]),
        # (["echo ../*"]),
        # (["echo ../../*"]),
        # (["echo src/*c"]),
    ],
)
def test_wildcards_with_bash(cmd):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert_same_lines_ordered(stdout_bash, stdout_minishell)
    assert len(stderr_minishell) == 0

    assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)


@pytest.mark.parametrize(
    "cmd, want",
    [
        (["echo ../*"], "../*"),
        (["echo ../../*"], "../../*"),
        (["echo src/*c"], "src/*c"),
    ],
)
def test_wildcards_divirgent_from_bash(cmd, want):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    cmd = "\n".join(cmd + ["echo $?\n"])

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )

    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert stdout_minishell[0] == want
    assert len(stderr_minishell) == 0

    assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)
