import subprocess


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
