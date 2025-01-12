import subprocess
import pytest
import time


@pytest.mark.parametrize(
    "cmd, err_msg",
    [
        (["wc < asdf"], "No such file or directory"),
        (
            ["wc < tests/end_to_end_tests/test_files/input1.txt > error/error"],
            "No such file or directory",
        ),
    ],
)
def test_redirection_errors(cmd, err_msg):
    minishell = subprocess.Popen(
        ["./minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    assert minishell.stdin is not None
    cmds = "\n".join(cmd + ["echo $?\n"])

    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())
    stdout_minishell = stderr_minishell.decode()
    stderr_minishell = stderr_minishell.decode()
    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell
    assert len(stderr_minishell) != 0
    assert err_msg in stderr_minishell
