# Minishell

<p align="center">
  <img src="https://user-images.githubusercontent.com/94384240/171533800-b1fa7318-f18e-44ba-a03f-69bb45900098.jpeg" alt="42 School Logo" width="300">
</p>

## 🖥️ 42 Cursus - Minishell

Minishell is a minimalistic shell project designed to mimic the behavior of a Unix shell. This project provides hands-on experience with process creation, pipes, redirections, signals, and command parsing.

It is one of the most complex projects in the 42 curriculum, reinforcing the fundamentals of operating systems and system calls.

📜 To see the project subject, [click here](https://github.com/jlebre/42Minishell/blob/main/minishell.pdf)!

---

## 📌 Project Objectives

✅ Implement a simple shell that executes commands like Bash.

✅ Handle multiple processes using `fork`, `execve`, and `wait`.

✅ Manage standard input/output redirections (`>`, `>>`, `<`).

✅ Support pipes (`|`) for command chaining.

✅ Implement built-in commands such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.

✅ Handle environment variables dynamically.

✅ Process user input with a command-line interface.

✅ Manage signals such as `Ctrl+C`, `Ctrl+D`, and `Ctrl+Z`.

✅ Maintain a command history.

---

## 📂 Important Files

🔹 [Makefile](https://github.com/jlebre/42Minishell/blob/main/Makefile)

🔹 [.h Header File](https://github.com/jlebre/42Minishell/blob/main/minishell.h)

🔹 [Main Shell Execution](https://github.com/jlebre/42Minishell/blob/main/srcs/main.c)

🔹 [Command Parser](https://github.com/jlebre/42Minishell/blob/main/parser)

🔹 [Built-in Commands](https://github.com/jlebre/42Minishell/blob/main/commands)

🔹 [Pipe Handler](https://github.com/jlebre/42Minishell/blob/main/pipes)

🔹 [Redirection Handler](https://github.com/jlebre/42Minishell/blob/main/redirections)

🔹 [Signal Handling](https://github.com/jlebre/42Minishell/blob/main/srcs/signals.c)

---

## 🚀 How to Use

### 1️⃣ Install Readline
```bash
sudo apt-get install -y libreadline-dev
```

### 2️⃣ Clone the Repository
```bash
git clone https://github.com/jlebre/42Minishell.git
cd 42Minishell
```

### 3️⃣ Compile the Shell
```bash
make
```

### 4️⃣ Run Minishell
```bash
./minishell
```

### Built-in Commands

🔹 `echo` - Print text to the terminal.

🔹 `cd` - Change directory.

🔹 `pwd` - Print working directory.

🔹 `export` - Set environment variables.

🔹 `unset` - Unset environment variables.

🔹 `env` - Display environment variables.

🔹 `exit` - Exit the shell.

### Special Key Combinations

🔹 `Ctrl+C` - Interrupt the current process.

🔹 `Ctrl+D` - Exit the shell.

🔹 `Ctrl+Z` - Suspend the current process.

---

![image](https://github.com/user-attachments/assets/e16df062-33bb-46b8-b261-1e201aed50e7)

