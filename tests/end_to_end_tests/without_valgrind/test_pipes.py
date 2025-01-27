#!/usr/bin/env python3

import pexpect
from conftest import cstm_expect, remove_cariage, remove_ansi_sequences


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

    bash.sendline("exit")
    bash.close()
    # bash_export_output = get_bash_export_output(bash, export_cmd)
    # bash.sendline("exit")
    # bash.close()

    # minishell = pexpect.spawn("./minishell", encoding="utf-8")
    # for cmd in precommands:
    #     cstm_expect(r"\$ ", minishell)
    #     minishell.sendline(cmd)
    # minishell_export_output = get_minishell_export_output(minishell, export_cmd)
    # minishell.sendline("exit")
    # minishell.close()

    # assert_equal_export(bash_export_output, minishell_export_output)
