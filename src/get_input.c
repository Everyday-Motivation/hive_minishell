/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:55:13 by jaeklee           #+#    #+#             */
/*   Updated: 2025/09/28 12:38:06 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_input(t_arena *arena, char **input)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
		line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		*input = NULL;
		return (0);
	}
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	*input = arena_strdup(arena, line,ft_strlen(line));
	if (!(*input))
	{
		if (isatty(STDIN_FILENO))
			free(line);
		return (0);
	}
	if (isatty(STDIN_FILENO))
		free(line);
	return (1);
}

int	check_input(t_arena *arena, char **input)
{
	int	len;

	if (!get_input(arena, input))
	{
		perror("get_input error");
		return (0);
	}
	len = ft_strlen(*input);
	if (len > 1024 || len <= 0)
	{
		perror("more_than Max input or allocation failed");
		return (0);
	}
	add_history(*input);
	return (1);
}

/*
파싱 파트 개발 진행 순서
1. leadline 호출하여 line 읽기
2. line을 적절하게 split 하기 (큰따옴표 작은따옴표 처리까지)
3. split 된 문자열에게 의미(type)를 부여하여 토큰화 시키는 과정 구현
4. 팀원과 함께 구문 (syntax) 문법 정의
5. 구문(syntax) 문법이 맞는지 체크하는 기능 구현
6. 구문(syntax) 문법에 맞추어 트리구조로 파싱 (재귀 하향 파서)
7. 전위 순회 탐색으로 명령어 프로세스 실행하도록 하기
8. 리다이렉션도 순차적으로 탐색하도록 구현
9. 파이프도 순차적으로 탐색하도록 구현
\ */
