import pytest
import pexpect
from conftest import cstm_expect, remove_cariage, remove_ansi_sequences, get_exit_status
import os


def remove_files(files):
    for f in files:
        if os.path.isfile(f):
            os.remove(f)


@pytest.mark.parametrize(
    "cmd",
    [
        ("echo Hello && echo World"),
        ("ls -l && echo hello"),
        ("ls -l && echo hello | cat"),
        ("ls -l && echo hello | cat"),
        ("cmd_fail && echo hello"),
        ("echo Hello || echo World"),
        ("cmd_fail || echo World"),
        ("cmd_fail || cmd_fail && cmd_fail"),
        ("echo Hello || echo New && echo World"),
        ("(echo Hello || echo New) && echo World"),
        ("echo Hello || (echo New && echo World)"),
        ("cmd_fail || echo New && echo World"),
        ("echo Hello || cmd_fail && echo World"),
        ("echo Hello || echo New && cmd_fail"),
        ("(cmd_fail || echo New) && echo World"),
        ("(echo Hello || cmd_fail) && echo World"),
        ("(echo Hello || echo New) && cmd_fail"),
        ("echo cmd_fail || (echo New && echo World)"),
        ("echo Hello || (cmd_fail && echo World)"),
        ("echo Hello || (echo New && cmd_fail)"),
        ("echo Hello && echo New || echo World"),
        ("(echo Hello && echo New) || echo World"),
        ("echo Hello && (echo New || echo World)"),
        ("cmd_fail && echo New || echo World"),
        ("echo Hello && cmd_fail || echo World"),
        ("echo Hello && echo New || cmd_fail"),
        ("(cmd_fail && echo New) || echo World"),
        ("(echo Hello && cmd_fail) || echo World"),
        ("(echo Hello && echo New) || cmd_fail"),
        ("echo cmd_fail && (echo New || echo World)"),
        ("echo Hello && (cmd_fail || echo World)"),
        ("echo Hello && (echo New || cmd_fail)"),
        ("cmd_fail || cmd_fail && echo World"),
        ("cmd_fail || echo New && cmd_fail"),
        ("echo hello || cmd_fail && cmd_fail"),
        ("(cmd_fail || cmd_fail) && echo World"),
        ("(cmd_fail || echo New) && cmd_fail"),
        ("(echo Hello || cmd_fail) && cmd_fail"),
        ("echo Hello || (cmd_fail && cmd_fail)"),
        ("cmd_fail || (cmd_fail && echo World)"),
        ("cmd_fail || (echo New && cmd_fail)"),
        ("cmd_fail && cmd_fail || echo World"),
        ("cmd_fail && echo New || cmd_fail"),
        ("echo hello && cmd_fail || cmd_fail"),
        ("(cmd_fail && cmd_fail) || echo World"),
        ("(cmd_fail && echo New) || cmd_fail"),
        ("(echo Hello && cmd_fail) || cmd_fail"),
        ("echo Hello && (cmd_fail || cmd_fail)"),
        ("cmd_fail && (cmd_fail || echo World)"),
        ("cmd_fail && (echo New || cmd_fail)"),
    ],
)
def test_logical(cmd):
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
    for line_bash, line_minishell in zip(bash_output, minishell_output):
        # if line_bash.startswith("bash"):
        #     line_bash = line_bash.split(": ")[-1]
        #     assert line_bash in line_minishell.lower()
        if "command not found" in line_bash:
            assert "Command not found" in line_minishell
        else:
            assert line_bash == line_minishell
    assert bash_exit_status == minishell_exit_status
