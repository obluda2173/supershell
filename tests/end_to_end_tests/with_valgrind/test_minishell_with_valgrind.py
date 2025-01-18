#!/usr/bin/env python3
import pytest

from assertions import (
    assert_no_memory_error_valgrind,
    assert_no_new_file_descriptors,
    assert_same_lines,
)
from conftest import (
    get_open_fds,
    get_prompt_minishell,
    parse_out_and_err_minishell,
    send_cmds_minishell,
    start_process_with_valgrind,
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
    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_bash, stdout_minishell)


def test_minishell_echo_wo_newline():
    cmd = "echo -n $PATH\n"

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )

    stdout_bash = stdout_bash.decode()
    prompt = get_prompt_minishell()
    stdout_minishell = stdout_minishell.decode().split("\n")[1][: -len(prompt)]
    stderr_minishell = stderr_minishell.decode()

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_bash, stdout_minishell)
