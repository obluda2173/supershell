import pytest
from conftest import (
    get_prompt_minishell,
    start_process,
    get_file_content,
    recreate_append_file,
    get_open_fds,
    send_cmds_minishell_with_open_fds,
    parse_out_and_err_minishell,
)
from assertions import (
    assert_no_memory_error_fsanitize,
    assert_same_lines_ordered,
    assert_no_new_file_descriptors,
)


@pytest.mark.parametrize(
    "cmd",
    [
        (['echo "hello" >> tests/end_to_end_tests/test_files/append.txt']),
    ],
)
def test_redirect_append(cmd):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    bash = start_process("bash")
    assert bash.stdin is not None

    recreate_append_file()
    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    append_path = "tests/end_to_end_tests/test_files/append.txt"
    file_bash = get_file_content(append_path)
    recreate_append_file()

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )
    file_minishell = get_file_content(append_path)
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )
    recreate_append_file()

    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert len(stderr_minishell) == 0
    assert_same_lines_ordered(stdout_minishell, stdout_bash)
    assert_same_lines_ordered(file_minishell, file_bash)

    # assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)


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
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    bash = start_process("bash")
    assert bash.stdin is not None

    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmd.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    tmp_path = "tests/end_to_end_tests/test_files/tmp.txt"
    file_bash = get_file_content(tmp_path)

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )
    file_minishell = get_file_content(tmp_path)

    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert len(stderr_minishell) == 0
    assert_same_lines_ordered(stdout_minishell, stdout_bash)

    assert len(file_bash) == len(file_minishell)
    for out1, out2 in zip(file_bash, file_minishell):
        assert out1 == out2, f"{out1} != {out2}"
    # assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)


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
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()
    minishell.communicate()

    cmd = "\n".join(cmd + ["echo $?\n"])

    bash = start_process("bash")
    assert bash.stdin is not None
    stdout_bash, _ = bash.communicate(cmd.encode())

    minishell = start_process("./minishell")
    stdout_minishell, stderr_minishell, open_fds_end = (
        send_cmds_minishell_with_open_fds(minishell, cmd)
    )

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_fsanitize(stdout_minishell, stderr_minishell)
    assert len(stderr_minishell) == 0
    assert_same_lines_ordered(stdout_minishell, stdout_bash)

    # assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)
