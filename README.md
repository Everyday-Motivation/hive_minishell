# Shell Process Controller

Implements a subset of a Unix shell: parsing, execution, pipes, redirections, heredoc, and basic builtins using `fork()`/`execve()` and signal handling.

## Description
`minishell` is a project that implements a simplified shell in order to more deeply understand Unix systems. It demonstrates parsing of commands, environment handling and expansion, heredoc, redirections, pipelines, child process management, and builtin commands while focusing on robustness and correct signal behaviour.

## Team
This project was developed by a two-person team:

**Jaekwang Robin Lee** [GitHub](https://github.com/turtleship11)
  - Responsibilities: parsing, heredoc process, and signal handling.

**Tiisetso Daniel Murray** [GitHub](https://github.com/Tiisetso)
  - Responsibilities: execution, builtins, signals, and testing.

## AI declaration
ChatGPT and Gemini were used to:
- search comprehensively for resources.
- review minor syntax challenges.
- assist creation of the bash test script.
- assist in creation of README.md

## Requirements
- Linux / POSIX environment
- GCC or clang
- make
- `readline` or custom input handling (if used)
- Optional: `valgrind` for memory checks.

## Build
```bash
make        # builds ./minishell
make clean  # removes object files
make fclean # removes binary as well
make re     # recompiles
```

## Usage
Run the shell:
```bash
./minishell
```
Then use it like a normal shell. Supported features include:
- Builtins: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`
- Pipelines and multiple commands: `ls | grep foo | wc -l`
- Redirections: `>`, `>>`, `<`
- Here-documents: `<< EOF` with proper signal handling
- Environment variable expansion

## Examples
```bash
echo "Hello world"
cd /tmp && ls -la
cat <<EOF | grep bar
line1
bar
EOF
```

## Notes
- Signals are handled to mimic typical shell behaviour (SIGINT, SIGQUIT).
- Child processes must be reaped correctly to avoid zombies.
- Parsing and tokenization must handle quotes and escapes correctly.

## Tests & Debugging
### Manual
Opening Bash in an adjacent terminal and conducting many tests for pipelines, redirections, heredoc, and builtin behaviour.
### Automated tester (shell_tester.sh)
A small test script `shell_tester.sh` runs a list of commands added line by line in `tests.txt` through both `bash` and your `minishell`, compares outputs (exact or normalised), and prints a summary of passed/similar/failed tests.

Usage:
```bash
# basic (uses defaults: tests.txt and ./minishell)
./shell_tester.sh
```

Notes:
- The script ignores blank lines and lines starting with `#` in the test file.
- The comparison includes a normalization step (whitespace, common shell prefixes) to catch "similar" outputs.
- The test file `tests.txt` contains many small command-line examples (quotes, expansion, redirection, pipes).

### Memory and file descriptor checks:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=readline.supp -s ./minishell
```

## Project Structure
```
minishell/
├── Makefile
├── README.md
├── include/
│   └── minishell.h
├── libft/            # bundled minimal libc helpers
├── src/
│   ├── builtins/
│   ├── execution/
│   ├── parsing/
│   ├── signal/
│   ├── utils/
│   └── minishell.c
└── obj/
```
