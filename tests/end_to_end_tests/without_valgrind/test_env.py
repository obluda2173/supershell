#!/usr/bin/env python3

import pytest
import pexpect
import os
from conftest import remove_cariage, remove_ansi_sequences, cstm_expect, get_exit_status


def get_bash_env_output(bash, line):
    cstm_expect(r"\$ ", bash)
    bash.sendline(line)
    cstm_expect(r"\$ ", bash)
    assert bash.before is not None
    bash_env_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in bash.before.split("\n")[1:-1]
    ]
    bash_env_output = [
        line
        for line in bash_env_output
        if not line.startswith("COLUMNS")
        and not line.startswith("_")
        and not line.startswith("_JAVA_AWT_WM_NONREPARENTING")
        and not line.startswith("DBUS_SESSION_BUS_ADDRESS")
        and not line.startswith("LS_COLORS")
        and not line.startswith("PATH")
        and not line.startswith("SHLVL")
        and not line.startswith("XMODIFIERS")
    ]
    return bash_env_output


def get_minishell_env_output(minishell, line):
    cstm_expect(r"\$ ", minishell)
    minishell.sendline(line)
    cstm_expect(r"\$ ", minishell)
    assert minishell.before is not None
    minishell_env_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]
    minishell_env_output = [
        line
        for line in minishell_env_output
        if not line.startswith("COLUMNS")
        and not line.startswith("_")
        and not line.startswith("_JAVA_AWT_WM_NONREPARENTING")
        and not line.startswith("DBUS_SESSION_BUS_ADDRESS")
        and not line.startswith("LS_COLORS")
        and not line.startswith("PATH")
        and not line.startswith("SHLVL")
        and not line.startswith("XMODIFIERS")
    ]
    return minishell_env_output


def assert_equal_env(bash_env_output, minishell_env_output):
    for line in bash_env_output:
        assert any(
            [line.startswith(line_m) for line_m in minishell_env_output]
        ), f"{line} not in minishell_env"

    for line in minishell_env_output:
        assert any(
            [line.startswith(line_b) for line_b in bash_env_output]
        ), f"{line} not in bash_env"
    assert len(bash_env_output) == len(minishell_env_output)


@pytest.mark.parametrize(
    "precommands, env_cmd",
    [
        ([], "env"),
        ([], "env < tests/end_to_end_tests/test_files/input1.txt"),
        (["env VAR1"], "env"),
        (["env VAR1="], "env"),
    ],
)
def test_env(precommands, env_cmd):
    bash = pexpect.spawn("./bash", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", bash)
        bash.sendline(cmd)
    bash_env_output = get_bash_env_output(bash, env_cmd)

    bash_exit_status = get_exit_status(bash)
    bash.sendline("exit")
    bash.close()

    minishell = pexpect.spawn("./minishell", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", minishell)
        minishell.sendline(cmd)
    minishell_env_output = get_minishell_env_output(minishell, env_cmd)
    minishell_exit_status = get_exit_status(minishell)

    minishell.sendline("exit")
    minishell.close()

    assert_equal_env(bash_env_output, minishell_env_output)
    assert bash_exit_status == minishell_exit_status
