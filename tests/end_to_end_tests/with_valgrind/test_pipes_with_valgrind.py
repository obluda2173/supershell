#!/usr/bin/env python3

import pytest
from conftest import remove_files
from assertions import assert_no_memory_error_valgrind, assert_no_open_fds_valgrind

import pexpect
from conftest import cstm_expect, remove_cariage, remove_ansi_sequences


@pytest.mark.parametrize(
    "cmd",
    [
        ("echo hello | cat\n"),
        ("echo hello2 | wc -c | cat \n"),
        ("echo hello2 | wc -c | ls -a | cat \n"),
        ("export VAR=asdf | echo $VAR"),
        ("export LOGNAME=asdf | echo $LOGNAME"),
        ("date | tee date_file | cut -d ' ' -f 1-3"),
        ("ls -l | tee ls_file | wc -l"),
        ('whoami | tee user_file | xargs echo "User is"'),
        ("date +%F | tee date_file | cut -d '-' -f 1"),
        ("uname -r | tee kernel_file | cut -d '.' -f 1"),
        ("echo hello | cat < no"),
        ("echo hello | asdf"),
        ("echo hello | cat | asdf"),
        ("export < non | echo hello"),
        ("echo hello | export < non"),
        ("echo hello | echo hello | export < non"),
        (
            "echo hello | echo hello | export < tests/end_to_end_tests/test_files/no_perm.txt"
        ),
    ],
)
def test_pipe_with_valgrind(cmd):
    minishell = pexpect.spawn(
        "valgrind --leak-check=full --track-fds=yes ./minishell", encoding="utf-8"
    )
    cstm_expect(r"\$ ", minishell)
    minishell.sendline(cmd)
    cstm_expect(r"\$ ", minishell)

    assert minishell.before is not None
    minishell_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]

    minishell.sendline("exit")
    minishell.close()

    assert_no_memory_error_valgrind("\n".join(minishell_output))
    assert_no_open_fds_valgrind(minishell_output)

    remove_files(["date_file", "kernel_file", "ls_file", "user_file"])
