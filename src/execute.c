
#include "../include/minishell.h"
#include <fcntl.h>

int execute_cmds(t_vec *cmds, t_vec *env)
{
    size_t i = 0;
    int pipe_fd[2];
    int prev_fd = -1; // 이전 명령의 stdout (pipe read)
    pid_t pid;
    t_cmd *cmd;

	(void)env;
    while (i < cmds->len)
    {
        cmd = (t_cmd *)ft_vec_get(cmds, i);

        // 마지막 명령이 아니라면 파이프 생성
        if (i < cmds->len - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return 0;
            }
        }

        pid = fork();
        if (pid == 0)
        {
            if (cmd->input_fd != 0)
                dup2(cmd->input_fd, STDIN_FILENO);
            else if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO); // 이전 파이프에서 입력 받기

            if (cmd->output_fd != 1)
                dup2(cmd->output_fd, STDOUT_FILENO);
            else if (i < cmds->len - 1)
                dup2(pipe_fd[1], STDOUT_FILENO); // 다음 명령에게 출력 넘김

            if (prev_fd != -1)
                close(prev_fd);
            if (i < cmds->len - 1)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            execvp(cmd->argv[0], cmd->argv);
            perror("execvp"); // 실패시
            _exit(1);
        }
        else if (pid < 0)
        {
            perror("fork");
            return 0;
        }

        if (prev_fd != -1)
            close(prev_fd);

        if (i < cmds->len - 1)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0]; 
        }

        i++;
    }

    while (wait(NULL) > 0);

    return 1;
}
