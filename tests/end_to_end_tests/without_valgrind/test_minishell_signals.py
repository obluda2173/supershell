#!/usr/bin/env python3


# Ctrl+C = SIGINT
# Ctrl+D = EOF

import pexpect
import os
import re
import time

from conftest import remove_cariage, remove_ansi_sequences

logname = os.getenv("LOGNAME", "")


def test_sigint_in_interactive_mode():
    # Start an interactive shell
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    # Expect the prompt (this may need to be adjusted depending on your environment)
    minishell.expect(r"\$ ")

    minishell.sendcontrol("c")
    time.sleep(0.01)

    assert minishell.isalive()

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    assert minishell.before is not None
    assert minishell.before[: len("^C")] == "^C"
    assert minishell.before[-len(f"\r\n{logname}") :] == f"\r\n{logname}"

    minishell.sendline("echo $?")

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    output = remove_ansi_sequences(minishell.before)
    output = output.split("\n")
    output = [remove_cariage(out) for out in output]
    code = int(output[1])
    assert code == 130

    minishell.sendline("exit")
    minishell.close()


def test_eof_in_cat_mode():
    # Start an interactive shell
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    # Expect the prompt (this may need to be adjusted depending on your environment)
    minishell.expect(r"\$ ")

    minishell.sendline("cat")
    time.sleep(0.01)
    minishell.sendcontrol("d")

    assert minishell.isalive()

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    assert minishell.before is not None
    output = remove_ansi_sequences(remove_cariage(minishell.before))
    assert output == f"cat\n{logname}"

    minishell.sendline("echo $?")

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    output = remove_cariage(remove_ansi_sequences(minishell.before))
    code = int(output.split("\n")[1])
    assert code == 0

    minishell.sendline("exit")
    minishell.close()


def test_sigint_in_hereloop_mode():
    # Start an interactive shell
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    # Expect the prompt (this may need to be adjusted depending on your environment)
    minishell.expect(r"\$ ")

    minishell.sendline("<<EOF\n")

    assert minishell.isalive()

    try:
        minishell.expect(r"heredoc> ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    minishell.sendcontrol("c")
    assert minishell.isalive()

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    assert minishell.before is not None
    output = remove_cariage(remove_ansi_sequences(minishell.before)).split("\n")
    assert output[1] == "^C"
    assert output[2] == f"{logname}"

    minishell.sendline("echo $?")

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    output = remove_ansi_sequences(minishell.before)
    output = output.split("\n")
    output = [remove_cariage(out) for out in output]
    code = int(output[1])
    assert code == 130
    minishell.sendline("exit")
    minishell.close()


def test_eof_in_hereloop_mode():
    # Start an interactive shell
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    # Expect the prompt (this may need to be adjusted depending on your environment)
    minishell.expect(r"\$ ")

    minishell.sendline("<<EOF\n")

    # try:
    #     minishell.expect(r"heredoc> ", timeout=0.1)
    # except pexpect.TIMEOUT:
    #     print("Timeout occurred")
    #     assert False, "Timeout occurred: warning: heredoc delimited by end-of-file"
    minishell.sendcontrol("d")

    assert minishell.isalive()

    try:
        minishell.expect(f"warning: heredoc delimited by end-of-file", timeout=2)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occurred: warning: heredoc delimited by end-of-file"

    minishell.sendline("exit")
    minishell.close()
