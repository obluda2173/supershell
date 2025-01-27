#!/usr/bin/env python3
import pytest
import pexpect
import os
from conftest import remove_cariage, remove_ansi_sequences

# from conftest import (
#     start_process,
#     send_cmds_minishell_with_open_fds,
#     parse_out_and_err_minishell,
#     get_open_fds,
# )
# from assertions import (
#     assert_no_memory_error_fsanitize,
#     assert_no_new_file_descriptors,
#     assert_no_memory_error_fsanitize,
# )


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


def cstm_expect(expr, shell):
    try:
        shell.expect(expr, timeout=0.5)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, f"Timeout occurred: warning: {expr}"


def get_bash_export_output(bash, line):
    cstm_expect(r"\$ ", bash)
    bash.sendline(line)
    cstm_expect(r"\$ ", bash)
    assert bash.before is not None
    export_vars_bash = [
        remove_cariage(remove_ansi_sequences(line))
        for line in bash.before.split("\n")[1:-1]
    ]
    export_vars_bash = [
        line
        for line in export_vars_bash
        if not line.startswith("declare -x COLUMNS")
        and not line.startswith("declare -x _JAVA_AWT_WM_NONREPARENTING")
    ]
    export_vars_bash = [
        line[: line.find('"') + 1] if line.find('"') > 0 else line
        for line in export_vars_bash
    ]
    return export_vars_bash


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
        line[: line.find('"') + 1] if line.find('"') > 0 else line
        for line in minishell_export_output
    ]
    return minishell_export_output


@pytest.mark.parametrize(
    "precommands, export_cmd",
    [
        ([], "export"),
        ([], "export < tests/end_to_end_tests/test_files/input1.txt"),
        (
            [],
            "export < non_existant",
        ),  # is an error case, needs to be handled seperately
        (["export VAR1"], "export"),
    ],
)
def test_export_with_redirection(precommands, export_cmd):
    bash = pexpect.spawn("./bash", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", bash)
        bash.sendline(cmd)
    bash_export_output = get_bash_export_output(bash, export_cmd)
    for l in bash_export_output:
        print(l)
    bash.sendline("exit")
    bash.close()

    print()
    minishell = pexpect.spawn("./minishell", encoding="utf-8")
    for cmd in precommands:
        cstm_expect(r"\$ ", minishell)
        minishell.sendline(cmd)
    minishell_export_output = get_minishell_export_output(minishell, export_cmd)
    for l in minishell_export_output:
        print(l)
    minishell.sendline("exit")
    minishell.close()

    assert_equal_export(bash_export_output, minishell_export_output)
