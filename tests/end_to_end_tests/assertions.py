def assert_no_memory_error(stdout_minishell, stderr_minishell):
    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell


def assert_same_lines(stdout_minishell, stdout_bash):
    assert len(stdout_bash) == len(stdout_minishell)
    for out1, out2 in zip(stdout_bash, stdout_minishell):
        assert out1 == out2, f"{out1} != {out2}"


def assert_no_new_file_descriptors(open_fds_beginning, open_fds_end):
    open_fds_beginning = [
        line for line in open_fds_beginning if (len(line) and "/usr/" not in line)
    ]
    open_fds_end = [
        line for line in open_fds_end if (len(line) and "/usr/" not in line)
    ]
    assert len(open_fds_beginning) == len(open_fds_end)
