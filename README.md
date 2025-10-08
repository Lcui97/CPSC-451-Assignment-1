# CPSC-451-Assignment-1
# Simple Shell

A very small UNIX-style shell written in **C++**.  
It repeatedly prints a prompt (`cmd> `), reads a command, forks a child process, runs the command using `execvp`, waits for the child to finish, and then prompts again.  
Type `exit` (or press `Ctrl+D`) to quit.

---

## Requirements
- A POSIX-compatible system (Linux, macOS, WSL on Windows works).
- A C++17 (or later) compiler such as `g++` or `clang++`.

---

##  Build

```bash
g++ -std=c++17 -Wall -Wextra -O2 -o shell shell.cpp
```

This will create an executable named `shell`.

---

## Run

```bash
./shell
```

You should see:

```
cmd>
```

Now you can type commands such as:

```
cmd> ls
cmd> pwd
cmd> date
cmd> echo hello world
```

When finished, type:

```
cmd> exit
```

or press `Ctrl+D` to quit.

