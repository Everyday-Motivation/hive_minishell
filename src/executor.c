
#include "../include/minishell.h"
#include <fcntl.h>

// You already have these types:
// typedef struct s_cmd   { t_vec args; t_vec redirs; } t_cmd;
// typedef struct s_redir { t_type type; char *data; } t_redir;
// enum t_type { WORD, D_Q, S_Q, S_LT, S_GT, D_LT, D_GT, PIPE };

// --- build_argv -------------------------------------------------------------
// Builds a NULL-terminated argv from c->args (vector of char*).
// Note: we *borrow* the char* pointers (no strdup). That’s fine since
// this runs in the child right before execvp().
char **build_argv(const t_cmd *c)
{
    size_t n = c->args.len;
    char **argv = (char **)calloc(n + 1, sizeof(char *));
    if (!argv) return NULL;

    for (size_t i = 0; i < n; ++i) {
        char **p = (char **)ft_vec_get((t_vec *)&c->args, i);
        argv[i] = *p; // borrow pointer
    }
    argv[n] = NULL;
    return argv;
}

// --- heredoc helper ---------------------------------------------------------
// Minimal heredoc: reads lines from the shell's stdin until `delim`,
// feeds them to the child via a pipe, and makes that pipe the child's stdin.
// This is simple and blocking; refine later for signals/$-expansion.
static int feed_heredoc_to_stdin(const char *delim)
{
    int h[2];
    if (pipe(h) < 0) return -1;

    pid_t pid = fork();
    if (pid < 0) {
        close(h[0]); close(h[1]);
        return -1;
    }
    if (pid == 0) {
        // Writer process
        close(h[0]);
        char *line = NULL;
        size_t cap = 0;
        ssize_t n;

        // Prompt only if tty; safe to omit prompts entirely if you prefer
        while (1) {
            if (isatty(STDIN_FILENO)) {
                write(STDERR_FILENO, "> ", 2);
            }
            n = getline(&line, &cap, stdin);
            if (n < 0) break;

            // Strip trailing newline for compare
            size_t len = (size_t)n;
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
                len--;
            }
            if (strcmp(line, delim) == 0) break;

            // Restore newline and write into pipe
            line[len] = '\n';
            len++;
            (void)!write(h[1], line, len);
        }
        free(line);
        close(h[1]);
        _exit(0);
    }

    // Child (reader): replace stdin with read end
    close(h[1]);
    if (dup2(h[0], STDIN_FILENO) < 0) {
        close(h[0]);
        return -1;
    }
    close(h[0]);

    // Wait for writer to finish (avoids zombie)
    int st;
    (void)waitpid(pid, &st, 0);
    return 0;
}

// --- apply_redirs -----------------------------------------------------------
// Applies all redirections in c->redirs to the *current process* (child).
// Returns 0 on success, -1 on error. Use before execvp().
int apply_redirs(const t_cmd *c)
{
    for (size_t i = 0; i < c->redirs.len; ++i) {
        t_redir *r = (t_redir *)ft_vec_get((t_vec *)&c->redirs, i);
        int fd = -1;

        switch (r->type) {
            case S_LT: { // <
                fd = open(r->data, O_RDONLY);
                if (fd < 0) return -1;
                if (dup2(fd, STDIN_FILENO) < 0) { close(fd); return -1; }
                close(fd);
            } break;

            case S_GT: { // >
                fd = open(r->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) return -1;
                if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return -1; }
                close(fd);
            } break;

            case D_GT: { // >>
                fd = open(r->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) return -1;
                if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return -1; }
                close(fd);
            } break;

            case D_LT: { // <<
                if (feed_heredoc_to_stdin(r->data) < 0) return -1;
            } break;

            default:
                // Ignore anything else; your parser shouldn't put others here
                break;
        }
    }
    return 0;
}

// ---------- minimal executor for RPN plan ----------
// Assumes: t_task { CMD|OP }, t_cmd (args, redirs), t_type (PIPE)
//          ft_vec_* available, and your apply_redirs() + build_argv() exist.


// If your headers already declare these, remove the prototypes:
extern int  apply_redirs(const t_cmd *c);
extern char **build_argv(const t_cmd *c);

// A pipeline "group": ordered list of commands to run left -> right
typedef struct s_group {
    t_vec cmds;   // element type: t_cmd
} t_group;

static int group_new(t_group *g) {
    return ft_vec_new(&g->cmds, 0, sizeof(t_cmd));
}
static int group_push_cmd(t_group *g, t_cmd *c) {
    return ft_vec_push(&g->cmds, c);
}
static int group_merge(t_group *dst, const t_group *left, const t_group *right) {
    for (size_t i = 0; i < left->cmds.len; ++i) {
        t_cmd *c = ft_vec_get((t_vec*)&left->cmds, i);
        if (!ft_vec_push(&dst->cmds, c)) return 0;
    }
    for (size_t i = 0; i < right->cmds.len; ++i) {
        t_cmd *c = ft_vec_get((t_vec*)&right->cmds, i);
        if (!ft_vec_push(&dst->cmds, c)) return 0;
    }
    return 1;
}

// Turn RPN plan -> single pipeline group using a tiny stack machine
static int build_group_from_rpn(const t_vec *plan, t_group *out_group) {
    t_vec stack; if (!ft_vec_new(&stack, 0, sizeof(t_group))) return 0;

    for (size_t i = 0; i < plan->len; ++i) {
        t_task *t = ft_vec_get((t_vec*)plan, i);

        if (t->action_type == CMD) {
            t_group g; if (!group_new(&g)) return 0;
            if (!group_push_cmd(&g, &t->u_action.cmd)) return 0;
            if (!ft_vec_push(&stack, &g)) return 0;
        } else { // OP
            if (t->u_action.op != PIPE) continue; // (extend later for &&, ||, ;, &)
            if (stack.len < 2) return 0;

            t_group right = *(t_group*)ft_vec_get(&stack, stack.len - 1);
            stack.len--;
            t_group left  = *(t_group*)ft_vec_get(&stack, stack.len - 1);
            stack.len--;

            t_group merged; if (!group_new(&merged)) return 0;
            if (!group_merge(&merged, &left, &right)) return 0;

            if (!ft_vec_push(&stack, &merged)) return 0;
            // (Optional) free left/right container vectors if you own deep copies
        }
    }

    if (stack.len != 1) return 0;
    *out_group = *(t_group*)ft_vec_get(&stack, 0);
    return 1;
}

// Run N>1 commands as a pipeline; N==1 falls back to run_single_cmd
static int run_pipeline_cmds(const t_vec *cmds, char **envp) {
    (void)envp; // using execvp; adjust if you use execve + PATH
    int prev_read = -1;
    pid_t last = -1;

    for (size_t i = 0; i < cmds->len; ++i) {
        int pfd[2] = { -1, -1 };
        int last_stage = (i == cmds->len - 1);
        if (!last_stage && pipe(pfd) < 0) return 1;

        pid_t pid = fork();
        if (pid == 0) {
            // Child: connect stdin from previous stage
            if (prev_read != -1) {
                dup2(prev_read, STDIN_FILENO);
            }
            // Child: connect stdout to next stage
            if (!last_stage) {
                close(pfd[0]);
                dup2(pfd[1], STDOUT_FILENO);
            }

            // Close fds we don't need after dup2
            if (prev_read != -1) close(prev_read);
            if (!last_stage) close(pfd[1]);

            // Apply per-command redirections
            t_cmd *c = ft_vec_get((t_vec*)cmds, i);
            if (apply_redirs(c) != 0) _exit(1);

            // Build argv and exec
            char **argv = build_argv(c);
            if (!argv || !argv[0]) _exit(127);
            execvp(argv[0], argv);
            _exit(127);
        }

        // Parent
        if (prev_read != -1) close(prev_read);
        if (!last_stage) {
            close(pfd[1]);
            prev_read = pfd[0];
        }
        last = pid;
    }

    // Wait (at least) for the last child; optionally wait for all
    int st; waitpid(last, &st, 0);
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
    return WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
}

// Single command runner (simple version: always fork + exec)
static int run_single_cmd(t_cmd *c, char **envp) {
    (void)envp;
    pid_t pid = fork();
    if (pid == 0) {
        if (apply_redirs(c) != 0) _exit(1);
        char **argv = build_argv(c);
        if (!argv || !argv[0]) _exit(127);
        execvp(argv[0], argv);
        _exit(127);
    }
    int st; waitpid(pid, &st, 0);
    return WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st);
}

static int run_group(t_group *g, char **envp) {
    if (g->cmds.len == 0) return 0;
    if (g->cmds.len == 1) {
        t_cmd *c = ft_vec_get(&g->cmds, 0);
        return run_single_cmd(c, envp);
    }
    return run_pipeline_cmds(&g->cmds, envp);
}

// ---------- call this from your loop ----------
void execute_plan(t_vec *plan, char **envp) {
    t_group g;
    if (!build_group_from_rpn(plan, &g)) {
        write(STDERR_FILENO, "plan error\n", 11);
        return;
    }
    (void)run_group(&g, envp);
    // If you own deep copies inside g.cmds, free them here.
}
