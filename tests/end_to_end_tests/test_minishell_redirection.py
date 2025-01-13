import pytest
import os
import time
import stat
from conftest import start_process, get_prompt_minishell


def check_permission(want_perm, file_path):
    mode = os.stat(file_path).st_mode

    user_perm = (
        4 * bool(mode & stat.S_IRUSR)
        + 2 * bool(mode & stat.S_IWUSR)
        + bool(mode & stat.S_IXUSR)
    )
    group_perm = (
        4 * bool(mode & stat.S_IRGRP)
        + 2 * bool(mode & stat.S_IWGRP)
        + bool(mode & stat.S_IXGRP)
    )
    other_perm = (
        4 * bool(mode & stat.S_IROTH)
        + 2 * bool(mode & stat.S_IWOTH)
        + bool(mode & stat.S_IXOTH)
    )
    got_perm = 100 * user_perm + 10 * group_perm + other_perm
    assert got_perm == want_perm


@pytest.mark.parametrize(
    "cmd",
    [
        (['echo "hello" > tests/end_to_end_tests/test_files/non_existent.txt']),
        (
            [
                "wc -c < tests/end_to_end_tests/test_files/input1.txt > tests/end_to_end_tests/test_files/non_existent.txt"
            ]
        ),
    ],
)
def test_out_redirections(cmd):
    bash = start_process("bash")
    assert bash.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, stderr_bash = bash.communicate(cmds.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    tmp_path = "tests/end_to_end_tests/test_files/non_existent.txt"
    assert os.path.isfile(tmp_path), f"file does not exist {tmp_path}"
    check_permission(644, tmp_path)
    with open(tmp_path, "r") as f:
        file_bash = f.readlines()
    os.remove(tmp_path)
    assert not os.path.isfile(tmp_path), f"file does still exist {tmp_path}"

    prompt = get_prompt_minishell()
    minishell = start_process("./minishell")
    assert minishell.stdin is not None
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    assert os.path.isfile(tmp_path), f"file does not exist {tmp_path}"
    check_permission(644, tmp_path)
    with open("tests/end_to_end_tests/test_files/non_existent.txt", "r") as f:
        file_minishell = f.readlines()
    os.remove("tests/end_to_end_tests/test_files/non_existent.txt")
    assert not os.path.isfile(tmp_path), f"file does still exist {tmp_path}"

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

    assert len(file_bash) == len(file_minishell)
    for out1, out2 in zip(file_bash, file_minishell):
        assert out1 == out2, f"{out1} != {out2}"


@pytest.mark.parametrize(
    "cmd",
    [
        (["wc -c < tests/end_to_end_tests/test_files/input1.txt"]),
        (["wc -c < tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt wc -c"]),
    ],
)
def test_in_redirections(cmd):
    minishell = start_process("./minishell")
    prompt, _ = minishell.communicate()
    prompt = prompt.decode()

    bash = start_process("bash")
    minishell = start_process("./minishell")

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
    ],
)
def test_redirection_errors(cmd, err_msg, want_exit_status):
    prompt = get_prompt_minishell()

    minishell = start_process("./minishell")
    assert minishell.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())
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
