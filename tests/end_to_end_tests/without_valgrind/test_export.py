#!/usr/bin/env python3
import pytest
import pexpect
import os
from conftest import remove_cariage, remove_ansi_sequences, cstm_expect


def assert_equal_export(bash_export_output, minishell_export_output):
    assert len(bash_export_output) == len(minishell_export_output)
    for line in bash_export_output:
        assert any(
            [line.startswith(line_m) for line_m in minishell_export_output]
        ), f"{line} not in minishell_export"

    for line in minishell_export_output:
        assert any(
            [line.startswith(line_b) for line_b in bash_export_output]
        ), f"{line} not in bash_export"


logname = os.getenv("LOGNAME")


def get_bash_export_output(bash, line):
    cstm_expect(r"\$ ", bash)
    bash.sendline(line)
    cstm_expect(r"\$ ", bash)
    assert bash.before is not None
    bash_export_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in bash.before.split("\n")[1:-1]
    ]
    bash_export_output = [
        line
        for line in bash_export_output
        if not line.startswith("declare -x COLUMNS")
        and not line.startswith("declare -x _JAVA_AWT_WM_NONREPARENTING")
        and not line.startswith("declare -x DBUS_SESSION_BUS_ADDRESS")
        and not line.startswith("declare -x LS_COLORS")
        and not line.startswith("declare -x PATH")
        and not line.startswith("declare -x SHLVL")
        and not line.startswith("declare -x XMODIFIERS")
    ]
    return bash_export_output


def get_minishell_export_output(minishell, line):
    cstm_expect(r"\$ ", minishell)
    minishell.sendline(line)
    cstm_expect(r"\$ ", minishell)
    assert minishell.before is not None
    minishell_export_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")[1:-1]
    ]
    minishell_export_output = [
        line
        for line in minishell_export_output
        if not line.startswith("declare -x COLUMNS")
        and not line.startswith("declare -x _JAVA_AWT_WM_NONREPARENTING")
        and not line.startswith("declare -x DBUS_SESSION_BUS_ADDRESS")
        and not line.startswith("declare -x LS_COLORS")
        and not line.startswith("declare -x PATH")
        and not line.startswith("declare -x SHLVL")
        and not line.startswith("declare -x XMODIFIERS")
    ]
    return minishell_export_output


@pytest.mark.parametrize(
    "precommands, export_cmd",
    [
        ([], "export"),
        ([], "export < tests/end_to_end_tests/test_files/input1.txt"),
        (["export VAR1"], "export"),
        (["export VAR1="], "export"),
        (["export VAR1=$LOGNAME"], "export"),
        (["export VAR1=$LOGNAME VAR2=$VAR1"], "export"),
        (["export VAR1=$LOGNAME VAR2=$VAR1 VAR3"], "export"),
        (["export LOGNAME=new_logname"], "export"),
        (["export VAR1=\"$LOGNAME\"'$LOGNAME'"], "export"),
        (["export VAR1=\"$LOGNAME=\"'$LOGNAME'"], "export"),
        (['export VAR1="$LOGNAME="\'LOG"inside"NAME\''], "export"),
        (["export VAR1=\"some 'inside' string\""], "export"),
        (["export VAR1=\"some 'inside' string\""], "export"),
        (["export VAR1='value'"], "export"),
        ([f"export VAR1={1000 * logname}"], "export"),
        (['export VAR="$USER"', "export VAR="], "export"),
        (['export VAR="$USER"', "export VAR"], "export"),
    ],
)
def test_export(precommands, export_cmd):
    bash = pexpect.spawn("./bash", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", bash)
        bash.sendline(cmd)
    bash_export_output = get_bash_export_output(bash, export_cmd)
    bash.sendline("exit")
    bash.close()

    minishell = pexpect.spawn("./minishell", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", minishell)
        minishell.sendline(cmd)
    minishell_export_output = get_minishell_export_output(minishell, export_cmd)
    minishell.sendline("exit")
    minishell.close()

    assert_equal_export(bash_export_output, minishell_export_output)


@pytest.mark.parametrize(
    "cmd,err_msg", [("export < non_existant", "No such file or directory")]
)
def test_export_errors(cmd, err_msg):
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    cstm_expect(r"\$ ", minishell)
    minishell.sendline(cmd)
    cstm_expect(r"\$ ", minishell)
    assert minishell.before is not None
    minishell_export_output = [
        remove_cariage(remove_ansi_sequences(line))
        for line in minishell.before.split("\n")
    ]

    assert err_msg in minishell_export_output[1]
    minishell.sendline("exit")
    minishell.close()
