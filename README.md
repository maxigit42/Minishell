# Minishell - As Beautiful as a Shell | 42 Madrid

*This project has been developed as part of the 42 curriculum by mwilline.*

## 🏆 Project Achievement: Building a UNIX Interpreter
Minishell is one of the most significant milestones in the 42 curriculum. It is a simplified implementation of a real shell (Bash), requiring a deep understanding of process synchronization, file descriptors, and signal handling in C.

This project represents over 4 months of intense work, transitioning from basic programming to complex system-level architecture.

---

## 🚀 Why this project demonstrates my technical level:
Building a shell from scratch is more than just "coding"; it's an engineering challenge that covers:
1.  **Process Management:** Using `fork()`, `waitpid()`, and `execve()` to manage the lifecycle of multiple child processes.
2.  **Inter-Process Communication (IPC):** Implementing a pipeline system (`|`) using `pipe()` and `dup2()` to redirect data streams between independent commands.
3.  **Complex Parsing:** Developing a Lexer and Parser capable of handling single quotes (`'`), double quotes (`"`), and environment variable expansion (`$VAR`).
4.  **Signal Handling:** Managing interactive interrupts (Ctrl-C, Ctrl-D, Ctrl-\) to match Bash's behavior without leaking memory or leaving zombie processes.

---

## 🏗️ Architecture & Logic
My Minishell is divided into four main modules:

1.  **The Lexer/Tokenizer:** Breaks the input string into meaningful tokens (Commands, Arguments, Pipes, Redirects).
2.  **The Parser:** Organizes tokens into an execution tree or list, handling the logic of redirections (`<`, `>`, `<<`, `>>`) and command sequences.
3.  **The Executor:** This is the core engine. It searches for executables in the `PATH`, manages the `forking` of processes, and connects them via pipes.
4.  **Built-ins:** Custom implementation of internal shell commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.



---

## 🧬 Data Engineering Relevance
* **Infrastructure Mastery:** In Data Engineering, Bash is the "glue" that connects containers and databases. Understanding how a shell handles environment variables and process streams is essential for building robust pipelines.
* **Resource Optimization:** Managing file descriptors and child processes directly taught me how to avoid resource leaks—a critical skill when processing Terabytes of data.
* **Automation Logic:** Every ETL job is essentially a sequence of commands handled by a shell. I now understand exactly what happens when a data script is executed.

---

## 🛠️ Features & Instructions

### Features Included:
- ✅ Full execution of external commands with paths (relative/absolute).
- ✅ Pipes `|` that connect the output of one command to the input of the next.
- ✅ Redirections `<` `>` `<<` `>>`.
- ✅ Environment variable expansion and `$?` exit status.
- ✅ Interactive signal handling.
- ✅ Functional history (using `readline`).

### Compilación y Ejecución
```bash
make
./minishell
