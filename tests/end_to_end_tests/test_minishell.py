#!/usr/bin/env python3
import subprocess
import pytest

@pytest.mark.parametrize("cmd", [
    (["ls -a", "ls -l"]),
    (["cat CMakeLists.txt"]),
    ])
def test_minishell(cmd):

    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    prompt, _ = minishell.communicate()
    prompt = prompt.decode()

    bash = subprocess.Popen(["bash"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    assert bash.stdin is not None
    assert minishell.stdin is not None

    cmds = "\n".join(cmd + ["echo $?"])

    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_minishell, _ = minishell.communicate(cmds.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell = [line for line in stdout_minishell.decode().split("\n") if not line.startswith(prompt)]

    assert len(stdout_bash) == len(stdout_minishell)
    for out1, out2 in zip(stdout_bash, stdout_minishell):
        assert out1 == out2, f"{out1} != {out2}"
