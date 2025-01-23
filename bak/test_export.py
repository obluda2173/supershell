#!/usr/bin/env python3
import pytest

from conftest import (
    start_process,
    send_cmds_minishell_with_open_fds,
    parse_out_and_err_minishell,
    get_open_fds,
)
from assertions import (
    assert_no_memory_error_fsanitize,
    assert_no_new_file_descriptors,
    assert_no_memory_error_fsanitize,
)


def assert_export_variables_same(stdout_minishell, stdout_bash):
    for out1 in stdout_minishell:
        assert any(
            [out1.split("=")[0] in line for line in stdout_bash]
        ), f"{out1} not in {stdout_bash}"

    for out1 in stdout_bash:
        assert any(
            [out1.split("=")[0] in line for line in stdout_minishell]
        ), f"{out1} not in {stdout_minishell}"

    assert len(stdout_bash) == len(stdout_minishell)


@pytest.mark.parametrize(
    "cmd",
    [
        (["export"]),
        # (["export < tests/end_to_end_tests/test_files/input1.txt"]),
        # (["export new", "export"]),
    ],
)
def test_export(cmd):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()

    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )
    [print(line) for line in stdout_minishell.decode().split("\n")]

    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )
    # print(stdout_bash)
    # print("hello")
    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert_export_variables_same(stdout_minishell, stdout_bash)
    assert "0" == stdout_minishell[-1]
    assert len(stderr_minishell) == 0

    # assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)
