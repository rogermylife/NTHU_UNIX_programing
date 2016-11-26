#  lab06_tsh

## Content
We will write a tiny shell (tsh) command processor like sh, bash, or csh for single line commands. Your shell's main loop will display a prompt, read a line of input, and fork a child process to perform the indicated command.

Required capabilities:

* Ordinary commands, consisting of an executable program name and an optional list of arguments, run in a separate process.

* Two built-in commands: cd and pwd 

* Background processing, when the last token in the command line is "&".

You should write tsh in C, using Unix system calls covered in our lectures. The main loop of your shell may look like this:

* Prints a prompt

* Reads a command line

* Parses the command line into tokens (words)

* Forks a child : loads and executes the command; // child process

* Waits for the child to terminate; // parent process

* Required error checking: Any command not found in one of the directories on $PATH.
