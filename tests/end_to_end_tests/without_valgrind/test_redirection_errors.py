#!/usr/bin/env python3

from conftest import (
    get_open_fds,
    parse_out_and_err_minishell,
    send_cmds_minishell,
    start_process,
)
import pytest

from assertions import (
    assert_no_memory_error_fsanitize,
    assert_no_new_file_descriptors,
)


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
        (
            [
                "cat tests/end_to_end_tests/test_files/input1.txt > tests/end_to_end_tests/test_files/no_perm.txt"
            ],
            "Permission denied",
            1,
        ),
        (
            ["cat <<EOF > tests/end_to_end_tests/test_files/no_perm.txt\nline1\nEOF\n"],
            "Permission denied",
            1,
        ),
        (
            ["wc <<EOF > error/error\nline1\nEOF\n"],
            "No such file or directory",
            1,
        ),
        (
            ["wc <<EOF >> error/error\nline1\nEOF\n"],
            "No such file or directory",
            1,
        ),
        (
            ["wc > error/error2 <<EOF >> error/error\nline1\nEOF\n"],
            "No such file or directory",
            1,
        ),
    ],
)
def test_redirection_errors(cmd, err_msg, want_exit_status):
    minishell = start_process("./minishell")
    open_fds_beginning = get_open_fds()

    cmd = "\n".join(cmd + ["echo $?\n"])
    stdout_minishell, stderr_minishell, open_fds_end = send_cmds_minishell(
        minishell, cmd
    )
    stdout_minishell, stderr_minishell = parse_out_and_err_minishell(
        stdout_minishell, stderr_minishell
    )

    assert_no_memory_error_fsanitize(stderr_minishell, stderr_minishell)
    assert len(stderr_minishell) != 0
    assert len(stdout_minishell) == 1
    assert err_msg in stderr_minishell
    assert want_exit_status == int(stdout_minishell[0])
    assert_no_new_file_descriptors(open_fds_beginning, open_fds_end)
