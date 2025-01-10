#!/usr/bin/env python3
import subprocess
import pytest

@pytest.mark.parametrize("cmd", [
    ("ls -a"),
    ("cat CMakeLists.txt"),
    ])
def test_minishell(cmd):
    bash = subprocess.Popen(["bash"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    # Wait for the process to complete
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_minishell, _ = minishell.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # always gives an empty line at the end
    stdout_minishell = stdout_minishell.decode().split("\n")[1:-1]

    assert len(stdout_bash) == len(stdout_minishell)
    for out1, out2 in zip(stdout_minishell, stdout_minishell):
        assert out1 == out2, f"{out1} != {out2}"
