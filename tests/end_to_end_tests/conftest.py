import subprocess
import stat
import os


def start_process(shell):
    return subprocess.Popen(
        [shell],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def get_prompt_minishell():
    minishell = start_process("./minishell")
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
