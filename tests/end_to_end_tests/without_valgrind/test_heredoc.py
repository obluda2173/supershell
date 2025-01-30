#!/usr/bin/env python3


import pexpect
from conftest import remove_cariage, remove_ansi_sequences, cstm_expect, get_exit_status


def test_cd():
    bash = pexpect.spawn("bash", encoding="utf-8")

    cstm_expect(r"\$ ", bash)
    bash.sendline("<<EOF")
    bash.sendline("'")
    bash.sendline("EOF")

    cstm_expect(r"\$ ", bash)
    assert bash.before is not None

    bash_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in bash.before.split("\n")[1:-1]
    ]
    print(bash_output)
    bash.sendline("exit")
    bash.close()
    assert False

    # minishell_output = []
    # minishell = pexpect.spawn("./minishell", encoding="utf-8")
    # cstm_expect(r"\$ ", minishell)
    # for cmd in commands:
    #     minishell.sendline(cmd)
    #     cstm_expect(r"\$ ", minishell)
    #     assert minishell.before is not None
    #     minishell_output += [
    #         remove_cariage(remove_ansi_sequences(line))
    #         for line in minishell.before.split("\n")[1:-1]
    #     ]

    # minishell_exit_status = get_exit_status(minishell)
    # minishell.sendline("exit")
    # minishell.close()

    # bash_output = [
    #     line if "error" not in line else "getcwd:" + line.split(":")[-1]
    #     for line in bash_output
    # ]

    # assert_equal_lines(bash_output, minishell_output)
    # assert bash_exit_status == minishell_exit_status
