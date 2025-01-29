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
        ("asdf && echo hello"),
        ("echo Hello || echo World"),
        ("asdf || echo World"),
        ("asdf || asdf && adsf"),
        ("echo Hello || echo New && echo World"),
        ("(echo Hello || echo New) && echo World"),
        ("echo Hello || (echo New && echo World)"),
        ("asdf || echo New && echo World"),
        ("echo Hello || asdf && echo World"),
        ("echo Hello || echo New && asdf"),
        ("(asdf || echo New) && echo World"),
        ("(echo Hello || asdf) && echo World"),
        ("(echo Hello || echo New) && asdf"),
        ("echo asdf || (echo New && echo World)"),
        ("echo Hello || (asdf && echo World)"),
        ("echo Hello || (echo New && asdf)"),
        ("echo Hello && echo New || echo World"),
        ("(echo Hello && echo New) || echo World"),
        ("echo Hello && (echo New || echo World)"),
        ("asdf && echo New || echo World"),
        ("echo Hello && asdf || echo World"),
        ("echo Hello && echo New || asdf"),
        ("(asdf && echo New) || echo World"),
        ("(echo Hello && asdf) || echo World"),
        ("(echo Hello && echo New) || asdf"),
        ("echo asdf && (echo New || echo World)"),
        ("echo Hello && (asdf || echo World)"),
        ("echo Hello && (echo New || asdf)"),
        ("asdf || asdf && echo World"),
        ("asdf || echo New && asdf"),
        ("echo hello || asdf && asdf"),
        ("(asdf || asdf) && echo World"),
        ("(asdf || echo New) && adsf"),
        ("(echo Hello || asdf) && adsf"),
        ("echo Hello || (asdf && asdf)"),
        ("asdf || (asdf && echo World)"),
        ("asdf || (echo New && asdf)"),
        ("asdf && asdf || echo World"),
        ("asdf && echo New || asdf"),
        ("echo hello && asdf || asdf"),
        ("(asdf && asdf) || echo World"),
        ("(asdf && echo New) || adsf"),
        ("(echo Hello && asdf) || adsf"),
        ("echo Hello && (asdf || asdf)"),
        ("asdf && (asdf || echo World)"),
        ("asdf && (echo New || asdf)"),
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
        if line_bash.startswith("bash"):
            line_bash = line_bash.split(": ")[-1]
            assert line_bash in line_minishell.lower()
        else:
            assert line_bash == line_minishell
    assert bash_exit_status == minishell_exit_status


# @pytest.mark.parametrize(
#     "cmd,err_msg,want_exit_status",
#     [
#         ("echo hello | cat < no", "No such file or directory", 1),
#         ("echo hello | asdf", "Command not found", 127),
#         ("echo hello | cat | asdf", "Command not found", 127),
#         ("export < non | echo hello", "No such file or directory", 0),
#         ("echo hello | export < non", "No such file or directory", 1),
#         ("echo hello | echo hello | export < non", "No such file or directory", 1),
#         (
#             "echo hello | echo hello | export < tests/end_to_end_tests/test_files/no_perm.txt",
#             "Permission denied",
#             1,
#         ),
#     ],
# )
# def test_pipe_errors(cmd, err_msg, want_exit_status):
#     minishell = pexpect.spawn("./minishell", encoding="utf-8")

#     cstm_expect(r"\$ ", minishell)
#     minishell.sendline(cmd)
#     cstm_expect(r"\$ ", minishell)

#     assert minishell.before is not None
#     minishell_output = [
#         remove_cariage(remove_ansi_sequences(line))
#         for line in minishell.before.split("\n")[1:-1]
#     ]
#     got_exit_status = get_exit_status(minishell)

#     minishell.sendline("exit")
#     minishell.close()

#     assert err_msg in minishell_output[0]
#     assert got_exit_status == want_exit_status
