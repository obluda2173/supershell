#!/usr/bin/env python3
import pytest

from tests.end_to_end_tests.assertions import (
    assert_no_memory_error,
    assert_no_new_file_descriptors,
    assert_same_lines,
)
from tests.end_to_end_tests.conftest import (
    get_open_fds,
    get_prompt_minishell,
    parse_out_and_err_minishell,
    send_cmds_minishell,
    start_process,
)


@pytest.mark.parametrize(
    "cmd",
    [
        (["echo hello"]),
        (["ls -a", "ls -l"]),
        (["cat CMakeLists.txt"]),
        (["echo $PATH"]),
        (["echo asdf $PATH asdf"]),
        (["echo  asdf    $PATH   asdf  "]),
        (['echo "$PATH"']),
        (["which echo"]),
        # (["echo *", "echo *"]),
    ],
)
def test_minishell(cmd):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_bash, stdout_minishell)
    assert len(stderr_minishell) == 0

    assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)


def test_minishell_echo_wo_newline():
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    cmd = "echo -n $PATH\n"

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )

    stdout_bash = stdout_bash.decode()
    prompt = get_prompt_minishell()
    stdout_minishell = stdout_minishell.decode().split("\n")[1][: -len(prompt)]
    stderr_minishell = stderr_minishell.decode()

    assert_no_memory_error(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_bash, stdout_minishell)
    assert len(stderr_minishell) == 0

    assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)
