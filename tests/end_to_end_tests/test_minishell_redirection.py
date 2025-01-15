import pytest
import os
from conftest import start_process, get_prompt_minishell, get_file_content


def recreate_append_file():
    try:
        os.remove("tests/end_to_end_tests/test_files/append.txt")
    except:
        pass
    with open("tests/end_to_end_tests/test_files/append.txt", "w") as file:
        file.write("append")


@pytest.mark.parametrize(
    "cmd",
    [
        (['echo "hello" >> tests/end_to_end_tests/test_files/append.txt']),
    ],
)
def test_redirect_append(cmd):
    bash = start_process("bash")
    assert bash.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])

    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    append_path = "tests/end_to_end_tests/test_files/append.txt"
    file_bash = get_file_content(append_path)
    recreate_append_file()

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    file_minishell = get_file_content(append_path)
    recreate_append_file()

    prompt = get_prompt_minishell()
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
        (['echo "hello" > tests/end_to_end_tests/test_files/tmp.txt']),
        (
            [
                "wc -c < tests/end_to_end_tests/test_files/input1.txt > tests/end_to_end_tests/test_files/tmp.txt"
            ]
        ),
        (["> tests/end_to_end_tests/test_files/tmp.txt"]),
    ],
)
def test_redirect_out(cmd):
    bash = start_process("bash")
    assert bash.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    tmp_path = "tests/end_to_end_tests/test_files/tmp.txt"
    file_bash = get_file_content(tmp_path)

    minishell = start_process("./minishell")
    assert minishell.stdin is not None
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    file_minishell = get_file_content(tmp_path)

    prompt = get_prompt_minishell()
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
    "cmd",
    [
        (["wc -c <<EOF\nline1\nline2\nEOF"]),
        (["cat <<EOF tests/end_to_end_tests/test_files/input1.txt\nline1\nline2\nEOF"]),
    ],
)
def test_heredoc_redirections(cmd):
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
