import pytest
from conftest import (
    get_prompt_minishell,
    start_process_with_valgrind,
    start_process,
    get_file_content,
    recreate_append_file,
    get_open_fds,
    send_cmds_minishell_with_open_fds,
    parse_out_and_err_minishell,
)
from assertions import (
    assert_no_memory_error_valgrind,
    assert_no_open_fds_valgrind,
    assert_same_lines,
)


@pytest.mark.parametrize(
    "cmd",
    [
        (['echo "hello" >> tests/end_to_end_tests/test_files/append.txt']),
    ],
)
def test_redirect_append(cmd):
    bash = start_process("bash")
    assert bash.stdin is not None

    recreate_append_file()
    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    append_path = "tests/end_to_end_tests/test_files/append.txt"
    file_bash = get_file_content(append_path)
    recreate_append_file()

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )
    file_minishell = get_file_content(append_path)
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )
    recreate_append_file()

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_minishell, stdout_bash)
    assert_same_lines(file_minishell, file_bash)


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

    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    tmp_path = "tests/end_to_end_tests/test_files/tmp.txt"
    file_bash = get_file_content(tmp_path)

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )
    file_minishell = get_file_content(tmp_path)

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_minishell, stdout_bash)

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
        (["wc -c <<EOF\nline1\nline2\nEOF"]),
        (["cat <<EOF tests/end_to_end_tests/test_files/input1.txt\nline1\nline2\nEOF"]),
        (
            [
                "cat < tests/end_to_end_tests/test_files/input1.txt <<EOF\nline1\nline2\nEOF"
            ]
        ),
        (
            [
                "cat <<EOF < tests/end_to_end_tests/test_files/input1.txt\nline1\nline2\nEOF"
            ]
        ),
    ],
)
def test_in_and_heredoc_redirections(cmd):
    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process_with_valgrind("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_valgrind(stdout_minishell, stderr_minishell)
    assert_no_open_fds_valgrind(stdout_minishell, stderr_minishell)
    assert_same_lines(stdout_minishell, stdout_bash)
