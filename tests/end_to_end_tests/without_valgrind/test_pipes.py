#!/usr/bin/env python3

import pytest
import pexpect
from conftest import (
    cstm_expect,
    remove_cariage,
    remove_ansi_sequences,
)


@pytest.mark.parametrize(
    "cmd",
    [
        ("echo hello | cat"),
        ("echo hello2 | cat"),
        ("echo hello2 | wc -c | cat | wc -c"),
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

    # bash_exit_status = get_exit_status(bash)

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

    # minishell_exit_status = get_exit_status(minishell)

    assert len(bash_output) == len(minishell_output)
    assert bash_output[0] == minishell_output[0]

    minishell.sendline("exit")
    minishell.close()
