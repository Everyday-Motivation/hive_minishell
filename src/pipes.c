// int main(int ac, char **av, char **envp)
// {
//     char    ***cmds;
//     int     n;

//     if (ac < 2)
//         perror_exit("args error");

//     // ex: av = {"./minishell", "ls -l", "|", "grep foo", "|", "wc -l"}
//     // cmds = { {"ls", "-l", NULL}, {"grep", "foo", NULL}, {"wc", "-l", NULL} }

//     execute_pipeline(cmds, n, envp);

//     return (0);
// } /// it's an example

void execute_pipeline(char ***cmds, int n, char **envp)
{
    int     i , j;
    int     pipe_fd[n-1][2];
    pid_t   pid;

    i = 0;
    while (i < n - 1)
    {
        if (pipe(pipe_fd[i]) == -1)
            perror_exit("pipe error");
        i++;
    }
    i = 0;

    while (i < n)
    {
        pid = fork();
        if (pid == -1)
            perror_exit("fork error");
        if (pid == 0)
        {
            if (i > 0)
                dup2(pipe_fd[i - 1][0], STDIN_FILENO); // check if it is first
            if (i < n - 1)
                dup2(pipe_fd[i][1], STDOUT_FILENO); // check if it's the last

            j = 0;
            while (j < n - 1)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
                j++;
            }

            execute(cmds[i][0], envp); 
            perror("exec failed");
            exit(1);
        }
        i++;
    }

	//parent process closing 
    i = 0;
    while (i < n - 1)
    {
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
        i++;
    }
    i = 0;
    while (i < n)
    {
        wait(NULL); // wait child p // we can get status as well (EXIT code)
        i++;
    }
}
// I think this logic should help you understand how multiple pipes work