#!/usr/bin/env python3
import pytest

from assertions import (
    assert_no_memory_error_valgrind,
    assert_no_open_fds_valgrind,
    assert_same_lines_ordered,
)
from conftest import (
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
        # (["echo *", "echo *"]),
    ],
)
def test_minishell(cmd):
    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell = send_cmds_minishell(minishell, cmd)

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines_ordered(stdout_bash, stdout_minishell)


def test_minishell_echo_wo_newline():
    cmd = "echo -n $PATH\n"

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell = send_cmds_minishell(minishell, cmd)

    stdout_bash = stdout_bash.decode()
    prompt = get_prompt_minishell()
    stdout_minishell = stdout_minishell.decode().split("\n")[1][: -len(prompt)]
    stderr_minishell = stderr_minishell.decode()

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines_ordered(stdout_bash, stdout_minishell)


@pytest.mark.parametrize(
    "cmd",
    [
        (["which echo"]),
        (["which echo < tests/end_to_end_tests/test_files/input1.txt"]),
        # (["echo *", "echo *"]),
    ],
)
def test_which_builtin(cmd):
    minishell = start_process_with_valgrind("./minishell")

    cmd = "\n".join(cmd + ["echo $?\n"])

    stdout_minishell, stderr_minishell = send_cmds_minishell(minishell, cmd)

    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert "minishell built-in command" in stdout_minishell[0]
    assert "0" == stdout_minishell[1]
