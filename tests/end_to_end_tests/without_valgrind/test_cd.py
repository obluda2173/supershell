#!/usr/bin/env python3


import pytest
import pexpect
import os
from conftest import remove_cariage, remove_ansi_sequences, cstm_expect, get_exit_status


def assert_equal_lines(bash_output, minishell_output):
    for line in bash_output:
        assert any(
            [line.startswith(line_m) for line_m in minishell_output]
        ), f"{line} not in minishell_output"

    for line in minishell_output:
        assert any(
            [line.startswith(line_b) for line_b in bash_output]
        ), f"{line} not in bash_output"
    assert len(bash_output) == len(minishell_output)


@pytest.mark.parametrize(
    "commands",
    [
        (["cd src", "ls -l"]),
        ("unset PWD", "cd .."),
        (
            "mkdir a",
            "mkdir a/b",
            "cd a/b",
            "rm -rf ../../a",
            "echo $?",
            "pwd",
            "cd ..",
            "pwd",
        ),
    ],
)
def test_cd(commands):
    bash_output = []
    bash = pexpect.spawn("bash", encoding="utf-8")
    cstm_expect(r"\$ ", bash)
    for cmd in commands:
        bash.sendline(cmd)
        cstm_expect(r"\$ ", bash)
        assert bash.before is not None
        bash_output += [
            remove_cariage(remove_ansi_sequences(line))
            for line in bash.before.split("\n")[1:-1]
        ]

    bash_exit_status = get_exit_status(bash)
    bash.sendline("exit")
    bash.close()

    minishell_output = []
    minishell = pexpect.spawn("./minishell", encoding="utf-8")
    cstm_expect(r"\$ ", minishell)
    for cmd in commands:
        minishell.sendline(cmd)
        cstm_expect(r"\$ ", minishell)
        assert minishell.before is not None
        minishell_output += [
            remove_cariage(remove_ansi_sequences(line))
            for line in minishell.before.split("\n")[1:-1]
        ]

    minishell_exit_status = get_exit_status(minishell)
    minishell.sendline("exit")
    minishell.close()

    bash_output = [
        line if "error" not in line else "getcwd:" + line.split(":")[-1]
        for line in bash_output
    ]

    assert_equal_lines(bash_output, minishell_output)
    assert bash_exit_status == minishell_exit_status
