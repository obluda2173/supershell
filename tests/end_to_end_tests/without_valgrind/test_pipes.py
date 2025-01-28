#!/usr/bin/env python3

import pytest
import pexpect
from conftest import cstm_expect, remove_cariage, remove_ansi_sequences, get_exit_status


@pytest.mark.parametrize(
    "cmd",
    [
        ("echo hello | cat"),
        ("echo hello2 | cat"),
        ("echo hello2 | wc -c | cat | wc -c"),
        ("export VAR=asdf | echo $VAR"),
        ("export LOGNAME=asdf | echo $LOGNAME"),
        # ("echo hello | cat < non_existant"),
        # ("ls | cat")
    ],
)
def test_pipes(cmd):
    bash = pexpect.spawn("./bash", encoding="utf-8")
    cstm_expect(r"\$ ", bash)
    bash.sendline(cmd)
    cstm_expect(r"\$ ", bash)

    assert bash.before is not None
    bash_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in bash.before.split("\n")[1:-1]
    ]

    bash_exit_status = get_exit_status(bash)

    bash.sendline("exit")
    bash.close()

    minishell = pexpect.spawn("./minishell", encoding="utf-8")
    cstm_expect(r"\$ ", minishell)
    minishell.sendline(cmd)
    cstm_expect(r"\$ ", minishell)

    assert minishell.before is not None
    minishell_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]

    minishell_exit_status = get_exit_status(minishell)

    minishell.sendline("exit")
    minishell.close()

    assert len(bash_output) == len(minishell_output)
    assert bash_output[0] == minishell_output[0]
    assert bash_exit_status == minishell_exit_status


@pytest.mark.parametrize(
    "cmd,err_msg,want_exit_status",
    [
        ("echo hello | cat < no", "No such file or directory", 1),
        ("echo hello | asdf", "Command not found", 127),
        ("echo hello | cat | asdf", "Command not found", 127),
        ("export < non | echo hello", "No such file or directory", 0),
        ("echo hello | export < non", "No such file or directory", 1),
        ("echo hello | echo hello | export < non", "No such file or directory", 1),
        (
            "echo hello | echo hello | export < tests/end_to_end_tests/test_files/no_perm.txt",
            "Permission denied",
            1,
        ),
    ],
)
def test_pipe_errors(cmd, err_msg, want_exit_status):
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    cstm_expect(r"\$ ", minishell)
    minishell.sendline(cmd)
    cstm_expect(r"\$ ", minishell)

    assert minishell.before is not None
    minishell_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]
    got_exit_status = get_exit_status(minishell)

    minishell.sendline("exit")
    minishell.close()

    assert err_msg in minishell_output[0]
    assert got_exit_status == want_exit_status
