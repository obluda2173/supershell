# Minishell  
Minishell is a simple UNIX shell implementation created as part of the 42 curriculum. It mimics the behavior of a basic shell, allowing users to execute commands, manage processes, and handle input/output operations.  
It was a pleasure to work with my teammate [Kay](https://github.com/Keisn1)

## 🛠️ Features  
- Executes built-in and external commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`).  
- Supports input (`<`) and output (`>` and `>>`) redirections.  
- Handles pipes (`|`) for command chaining.
- Handles all kind of wildcards (`*`).
- Manages environment variables.  
- Implements signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`).  
- Displays proper error messages.

## ⚙️ Built-in Commands
Command | Description
:----------- | :-----------:
echo | Prints output to the terminal
cd | Changes the current directory
pwd | Displays the current directory
export | Sets environment variables
unset | Unsets environment variables
env | Displays environment variables
exit | Exits the shell

## 🛡️ Error Handling
- Proper error messages for invalid commands or syntax.
- Graceful handling of system call failures.
- Handling of Ctrl+C, Ctrl+D, and Ctrl+\.

## 🧠 Learning Outcomes
- Process management using fork(), execve(), waitpid().
- Inter-process communication with pipes.
- Signal handling with signal().
- Advanced memory management techniques.
