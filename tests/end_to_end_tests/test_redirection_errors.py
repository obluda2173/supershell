#!/usr/bin/env python3

from conftest import start_process, get_prompt_minishell
import pytest


@pytest.mark.parametrize(
    "cmd, err_msg, want_exit_status",
    [
        (["wc < asdf"], "No such file or directory", 1),
        (
            ["wc < tests/end_to_end_tests/test_files/input1.txt > error/error"],
            "No such file or directory",
            1,
        ),
        (["<"], "parsing error redirection", 2),
        (['echo "hello" | <'], "parsing error redirection", 2),
        (["< file.txt"], "No such file or directory", 1),
        (
            ["< tests/end_to_end_tests/test_files/no_perm.txt"],
            "Permission denied",
            1,
        ),
    ],
)
def test_redirction_errors(cmd, err_msg, want_exit_status):
    prompt = get_prompt_minishell()

    minishell = start_process("./minishell")
    assert minishell.stdin is not None

    cmd = "\n".join(cmd + ["echo $?\n"])
    minishell.stdin.write(cmd.encode())
    minishell.stdin.flush()
    stdout_minishell, stderr_minishell = minishell.communicate()
    stdout_minishell = [
        line
        for line in stdout_minishell.decode().split("\n")
        if not (line.startswith(prompt) or line.startswith("heredoc>"))
    ]
    stderr_minishell = stderr_minishell.decode()

    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell
    assert len(stderr_minishell) != 0
    assert len(stdout_minishell) == 1
    assert err_msg in stderr_minishell
    assert want_exit_status == int(stdout_minishell[0])
