#include "../include/minishell.h"
#include <fcntl.h>



static int handle_redirection(t_arena *arena, t_token *tok, t_token *next, t_cmd *cmd)
{
	(void)arena;
	
	if (!next || next->type != WORD)
		return 0;

	if (tok->type == S_LT)
		cmd->input_fd = open(next->data, O_RDONLY);
	else if (tok->type == S_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (tok->type == D_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (tok->type == D_LT)
	{
		cmd->heredoc = 1;
		//  heredoc 처리
	}
	if ((tok->type == S_LT || tok->type == S_GT || tok->type == D_GT) && (cmd->input_fd == -1 || cmd->output_fd == -1))
	{
		perror("open");
		return 0;
	}
	return 1;
}
static int parse_single_command(t_arena *arena, t_vec *tokens, size_t *i, t_cmd *cmd)
{
	t_token *tok;
	t_vec argv;

	cmd->input_fd = 0;
	cmd->output_fd = 1;
	cmd->heredoc = 0;

	if (ft_vec_new(&argv, 0, sizeof(char *)) < 0)
		return 0;

	while (*i < tokens->len)
	{
		tok = (t_token *)ft_vec_get(tokens, *i);

		if (tok->type == PIPE)
		{
			(*i)++;
			break;
		}
		else if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT || tok->type == D_GT)
		{
			t_token *next = ft_vec_get(tokens, *i + 1);
			if (!handle_redirection(arena, tok, next, cmd))
				return 0;
			(*i) += 2;
		}
		else if (tok->type == WORD)
		{
			char *arg = arena_strdup(arena, tok->data, strlen(tok->data));
			ft_vec_push(&argv, &arg);
			(*i)++;
		}
		else
		{
			(*i)++;
		}
	}

	char **args = arena_alloc(arena, sizeof(char *) * (argv.len + 1));
	for (size_t j = 0; j < argv.len; j++)
		args[j] = *(char **)ft_vec_get(&argv, j);
	args[argv.len] = NULL;
	cmd->argv = args;

	return 1;
}

int parse_tokens(t_arena *arena, t_vec *tokens, t_vec *cmds)
{
	t_cmd cmd;
	size_t i = 0;

	if (ft_vec_new(cmds, 0, sizeof(t_cmd)) < 0)
		return 0;

	while (i < tokens->len)
	{
		if (!parse_single_command(arena, tokens, &i, &cmd))
			return 0;

		ft_vec_push(cmds, &cmd);
	}

	return 1;
}

// int parse_tokens(t_arena *arena, t_vec *tokens, t_vec *cmds)
// {
//     t_cmd cmd;
//     t_token *tok;
//     t_vec argv;
//     size_t i = 0;

//     if (ft_vec_new(cmds, 0, sizeof(t_cmd)) < 0)
//         return 0;

//     while (i < tokens->len)
//     {
//         cmd.input_fd = 0;
//         cmd.output_fd = 1;
//         cmd.heredoc = 0;
//         ft_vec_new(&argv, 0, sizeof(char *));

//         while (i < tokens->len)
//         {
//             tok = (t_token *)ft_vec_get(tokens, i);

//             if (tok->type == PIPE)
//             {
//                 i++;
//                 break;
//             }
//             else if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT || tok->type == D_GT)
//             {
//                 t_token *next = ft_vec_get(tokens, i + 1);
//                 if (!next || next->type != WORD)
//                     return 0; // 에러: 리디렉션 뒤에 파일 없음

//                 if (tok->type == S_LT)
//                     cmd.input_fd = open(next->data, O_RDONLY);
//                 else if (tok->type == S_GT)
//                     cmd.output_fd = open(next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                 else if (tok->type == D_GT)
//                     cmd.output_fd = open(next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
//                 else if (tok->type == D_LT)
//                 {
//                     cmd.heredoc = 1;
//                     // heredoc 처리 필요 (나중에 구현)
//                 }

//                 i += 2;
//             }
//             else if (tok->type == WORD)
//             {
//                 char *arg = arena_strdup(arena, tok->data, strlen(tok->data));
//                 ft_vec_push(&argv, &arg);
//                 i++;
//             }
//             else
//             {
//                 i++; // 기타 토큰은 무시
//             }
//         }

//         // NULL-terminated argv 만들기
//         char **args = arena_alloc(arena, sizeof(char *) * (argv.len + 1));
//         for (size_t j = 0; j < argv.len; j++)
//             args[j] = *(char **)ft_vec_get(&argv, j);
//         args[argv.len] = NULL;

//         cmd.argv = args;
//         ft_vec_push(cmds, &cmd);
//     }

//     return 1;
// }
