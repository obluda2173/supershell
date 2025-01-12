#!/usr/bin/env python3
import subprocess
import pytest
import time


@pytest.mark.parametrize(
    "cmd",
    [
        (["ls -a", "ls -l"]),
        (["cat CMakeLists.txt"]),
        (["echo $PATH"]),
        (["echo asdf $PATH asdf"]),
        (["echo  asdf    $PATH   asdf  "]),
        (['echo "$PATH"']),
        (["wc -c < tests/end_to_end_tests/test_files/input1.txt"]),
        (["wc -c < tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt wc -c"]),
        # (["<<EOF", "line1", "line2", "EOF"]),
        # (["<< EOF", "line1", "line2"]),
        # (["<< asdf", "line1", "line2"]),
        # (["<< 'asdf'", "line1", "line2"]),
        # (["<<    'EOF'", "line1", "line2"]),
        # (["<<    'EOF'", "line1", "line2"]),
    ],
)
def test_minishell(cmd):

    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    prompt, _ = minishell.communicate()
    prompt = prompt.decode()

    bash = subprocess.Popen(
        ["bash"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )

    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    assert bash.stdin is not None
    assert minishell.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])

    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell = [
        line
        for line in stdout_minishell.decode().split("\n")
        if not (line.startswith(prompt) or line.startswith("heredoc>"))
    ]
    stderr_minishell = stderr_minishell.decode()

    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell
    assert len(stdout_bash) == len(stdout_minishell)
    assert len(stderr_minishell) == 0
    for out1, out2 in zip(stdout_bash, stdout_minishell):
        assert out1 == out2, f"{out1} != {out2}"
