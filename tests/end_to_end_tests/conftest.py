import subprocess
import stat
import os
import time
import re
import pexpect


def start_process(shell):
    return subprocess.Popen(
        [shell],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def start_process_with_valgrind(shell):
    return subprocess.Popen(
        ["valgrind", "--leak-check=full", "--track-fds=yes", shell],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def get_prompt_minishell(minishell="./minishell"):
    minishell = start_process(minishell)
    prompt, _ = minishell.communicate()
    return prompt.decode()


def check_permission(want_perm, file_path):
    mode = os.stat(file_path).st_mode

    user_perm = (
        4 * bool(mode & stat.S_IRUSR)
        + 2 * bool(mode & stat.S_IWUSR)
        + bool(mode & stat.S_IXUSR)
    )
    group_perm = (
        4 * bool(mode & stat.S_IRGRP)
        + 2 * bool(mode & stat.S_IWGRP)
        + bool(mode & stat.S_IXGRP)
    )
    other_perm = (
        4 * bool(mode & stat.S_IROTH)
        + 2 * bool(mode & stat.S_IWOTH)
        + bool(mode & stat.S_IXOTH)
    )
    got_perm = 100 * user_perm + 10 * group_perm + other_perm
    assert got_perm == want_perm


def get_file_content(path):
    assert os.path.isfile(path), f"file does not exist {path}"
    check_permission(644, path)
    with open(path, "r") as f:
        file_content = f.readlines()
    os.remove(path)
    assert not os.path.isfile(path), f"file does still exist {path}"
    return file_content


def recreate_append_file():
    try:
        os.remove("tests/end_to_end_tests/test_files/append.txt")
    except:
        pass
    with open("tests/end_to_end_tests/test_files/append.txt", "w") as file:
        file.write("append")


def get_open_fds():
    lsof_process = subprocess.Popen(
        ["lsof", "-c", "minishell"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    assert lsof_process.stdin is not None
    open_fds, _ = lsof_process.communicate()
    open_fds = open_fds.decode().split("\n")
    return open_fds


def send_cmds_minishell_with_open_fds(minishell, cmd):
    assert minishell.stdin is not None
    minishell.stdin.write(cmd.encode())
    minishell.stdin.flush()
    time.sleep(0.1)  # give the OS time to close the file descriptor
    open_fds_end = get_open_fds()
    stdout_minishell, stderr_minishell = minishell.communicate()
    return stdout_minishell, stderr_minishell, open_fds_end


def send_cmds_minishell(minishell, cmd):
    assert minishell.stdin is not None
    minishell.stdin.write(cmd.encode())
    minishell.stdin.flush()
    stdout_minishell, stderr_minishell = minishell.communicate()
    return stdout_minishell, stderr_minishell


def parse_out_and_err_minishell(
    stdout_minishell, stderr_minishell, minishell="./minishell"
):
    prompt = get_prompt_minishell(minishell)
    stdout_minishell = [
        line
        for line in stdout_minishell.decode().split("\n")
        if not (line.startswith(prompt) or line.startswith("heredoc>"))
    ]
    stderr_minishell = stderr_minishell.decode()
    return stdout_minishell, stderr_minishell


def remove_cariage(text):
    ansi_escape = re.compile(
        r"""\r""",
        re.VERBOSE,
    )

    return ansi_escape.sub("", text)


def remove_tab(text):
    ansi_escape = re.compile(
        r"""\t""",
        re.VERBOSE,
    )

    return ansi_escape.sub("", text)


def remove_ansi_sequences(text):
    ansi_escape = re.compile(
        r"""
        \x1B  # ESC
        (?:   # 7-bit C1 Control Sequence
            [@-Z\\-_]
        |     # or [ for CSI
            \[
            [0-?]*  # Parameter bytes
            [ -/]*  # Intermediate bytes
            [@-~]   # Final byte
        )
    """,
        re.VERBOSE,
    )

    return ansi_escape.sub("", text)


def cstm_expect(expr, shell):
    try:
        shell.expect(expr, timeout=1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, f"Timeout occurred: warning: {expr}"


def get_exit_status(shell):
    shell.sendline("echo $?")
    cstm_expect(r"\$ ", shell)

    assert shell.before is not None
    return int(remove_ansi_sequences(remove_cariage(shell.before.split("\n")[1])))


def remove_files(files):
    for f in files:
        if os.path.isfile(f):
            os.remove(f)
