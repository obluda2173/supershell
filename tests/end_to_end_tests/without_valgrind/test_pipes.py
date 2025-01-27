#!/usr/bin/env python3

import pexpect
from conftest import cstm_expect, remove_cariage, remove_ansi_sequences, get_exit_status


def test_pipes():
    line = "echo hello | cat"
    bash = pexpect.spawn("./bash", encoding="utf-8")
    cstm_expect(r"\$ ", bash)
    bash.sendline(line)
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
    minishell.sendline(line)
    cstm_expect(r"\$ ", minishell)

    assert minishell.before is not None
    minishell_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]

    # minishell_exit_status = get_exit_status(minishell)

    assert len(bash_output) == len(minishell_output)

    minishell.sendline("exit")
    minishell.close()
    # minishell_export_output = get_minishell_export_output(minishell, export_cmd)
    # minishell.sendline("exit")
    # minishell.close()

    # minishell = pexpect.spawn("./minishell", encoding="utf-8")
    # for cmd in precommands:
    #     cstm_expect(r"\$ ", minishell)
    #     minishell.sendline(cmd)
    # minishell_export_output = get_minishell_export_output(minishell, export_cmd)
    # minishell.sendline("exit")
    # minishell.close()

    # assert_equal_export(minishell_export_output, minishell_export_output)
