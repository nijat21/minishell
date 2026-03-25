*This project has been created as part of the 42 curriculum by otlacerd, nijat.*

# Minishell

## Description

Minishell is a simplified Unix shell implementation developed as part of the 42 curriculum. The objective of this project is to reproduce the core behavior of a standard shell (such as Bash), including command execution, process control, and input/output redirection.

The project is divided into two main components:

* **Parsing (by Nijat):**
  Implemented using a tokenization-based approach, focusing on correctness, robustness, and strict handling of shell syntax. The parser builds structured representations of commands, pipelines, and redirections.

* **Execution (by Otávio):**
  Developed with a custom approach prioritizing behavioral fidelity to Bash. Execution logic handles processes, pipes, redirections, environment variables, and edge cases with a focus on correctness and efficiency.

The shell supports:

* Command execution with arguments
* Pipes (`|`)
* Redirections (`>`, `<`, `>>`, `<<`)
* Environment variable expansion
* Built-in commands

Also includes several advanced behaviors beyond the mandatory scope, like update of "`_=`" variable, non-interactive exception behavior, update of "`PWD`" when cd doesnt find the folder, heredoc-history with personal upgrade for the comand-line, special message when exiting heredoc with Control+D with count lines, Core_Dump message troughout the the signal, export with "`+=`" feature, multiple heredoc-files to preserve content in multiple heredoc redirection "`<<` + `>` | `etc`"

---

## Features

* Fully functional built-in commands:

  * `cd`
  * `echo`
  * `env`
  * `export`
  * `unset`
  * `pwd`
  * `exit`

* Advanced execution handling:

  * Proper pipeline management using multiple processes
  * Accurate reproduction of Bash exit statuses
  * Detection of interactive vs non-interactive mode (e.g., `cmd | ./minishell`)
  * Correct signal handling and error messaging

* Redirections:

  * Input/output redirection
  * Append mode
  * Heredoc with history support

* Environment system:

  * Dynamically managed environment array
  * Efficient updates and expansions

* Data structures:

  * Main structure based on linked lists
  * Each node represents a pipeline segment
  * Redirections stored as a secondary linked list
  * Use of enums for redirection types for clarity and efficiency

---

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./minishell
```
### Execution with valgrind
```bash
make val
```

### Usage

* Run commands as in a normal shell:

```bash
ls -l | grep .c
```

* Use redirections:

```bash
cat file.txt > output.txt
```

* Use heredoc:

```bash
cat << EOF
```

---

## Project Structure

* `src/parsing/` → Tokenization, syntax checking, pipeline construction
* `src/execution/` → Process execution, pipes, redirections, built-ins
* `includes/` → Header files and data structures
* `general_utils/` → Utility functions
* `Libft/` → Custom standard library implementation

---

## Technical Choices

* **Tokenization-based parsing:** ensures strict control over syntax and expansion
* **Linked list architecture:** flexible and efficient representation of pipelines
* **Dynamic environment array:** optimized environment variable management
* **Custom execution flow:** allows fine-grained control over process behavior
* **Enum-based redirection types:** improves readability and performance

---

## Resources

### Documentation

* POSIX Shell Documentation
* Linux Manual Pages (`man execve`, `man fork`, `man waitpid`)
* Bash behavior references and testing

### Articles & Tutorials

* Process management in Unix systems
* File descriptor and pipe handling
* Signal handling in C

### AI Usage

AI was used as a supporting tool for:

* Clarifying low-level system call behavior.
* Debugging specific edge cases related to bash.
* Getting reference of bash's functionality.

All core logic, architecture, and implementation decisions were independently designed and developed.

---

## Notes

This project emphasizes precision in reproducing Bash behavior, including subtle edge cases in execution, signal handling, and error reporting. It reflects a strong focus on system-level programming and deep understanding of Unix process control.
