/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 14:44:07 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

  char **copy_env(char **envp)
  {
	char **env;
	int len = 0;
	int i = 0;
		
	while(*env != '\0')
		len++;
	env = malloc((len + 1) * sizeof(char *));
	if (!env)
		return NULL;
	while (i < len)
	{
		env = ft_strdup(i);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return env;
  }

int init_env(char **env, char **envp)
{
	*env = copy_env(envp);
	if (!*env)
		return (-1);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char **env;

	env = copy_env(envp);
	if (init_env(&env, envp) < 0)
	{
		perror("Environment init failed");
		return (1);
	}
	/// signal init
	// get input -> check if it is valid input 
	// -> if yes ->parsing  ->>> first tokenizing 
	
	
	return (0);
}

// #define BUFFER_SIZE 1024

// int main(int ac, char **av, char **envp) {
//     (void)ac;
//     (void)av;
//     (void)envp;

//     char input[BUFFER_SIZE];

//     write(STDOUT_FILENO, "$ ", 2);

//     if (get_input(input, sizeof(input)) == -1) {
//         return 1;
//     }

//     char **tokens = ft_tokenize(input);

//     if (tokens) {
//         int i = 0;
//         while (tokens[i] != NULL) {
//             printf("Token[%d]: %s\n", i, tokens[i]);
//             i++;
//         }

//         ft_free_tokens(tokens);
//     }

//     return 0;
// }

// int get_input(char *input, size_t size) {
//     ssize_t bytes_read = read(STDIN_FILENO, input, size - 1);
//     if (bytes_read == -1) {
//         perror("read");
//         return -1;
//     }
//     input[bytes_read] = '\0';

//     // 개행 문자 제거
//     size_t len = strlen(input);
//     if (len > 0 && input[len - 1] == '\n') {
//         input[len - 1] = '\0';
//     }
//     return 0;
// }

// // 입력을 공백 기준으로 토큰화하는 함수
// char **ft_tokenize(char *input) {
//     char **tokens;
//     char *token;
//     int count = 0;

//     tokens = malloc(sizeof(char *) * 10);  // 토큰의 최대 개수 10개로 시작

//     if (!tokens)
//         return NULL;

//     token = input;
//     while (token != NULL && *token != '\0') {
//         while (*token == ' ' || *token == '\t') token++;  // 앞의 공백은 건너뛰기
//         if (*token == '\0') break;

//         char *start = token;
//         while (*token != ' ' && *token != '\t' && *token != '\0') token++;  // 단어 끝까지

//         size_t length = token - start;
//         char *word = malloc(length + 1);
//         if (!word) return NULL;
//         strncpy(word, start, length);
//         word[length] = '\0';

//         tokens[count] = word;
//         count++;

//         if (count % 10 == 0) {
//             tokens = realloc(tokens, sizeof(char *) * (count + 10));
//             if (!tokens) return NULL;
//         }
//     }

//     tokens[count] = NULL;
//     return tokens;
// }

// void ft_free_tokens(char **tokens) {
//     int i = 0;
//     while (tokens[i] != NULL) {
//         free(tokens[i]);
//         i++;
//     }
//     free(tokens);
// }


/*
int a[100];
int *a;

b = malloc(100 * sizeof(int));

a[0] = 1;
a[2] = 100;

a[101] //error

temp = malloc(101 * sizeof(int));
while(i)
{
	temp[i] = a[i];
	a = temp;
	free(temp);
	i++;
}

struct t_token t[100]; //stack
stuct t_token *j;

j = malloc(100 * sizeof(t_token));
if(!j)
	return (-1);

j[0].val = 1;

j[101]// error seg

$> cat file.txt | > test.txt

"cat file.txt | > test.txt"
ft_split("cat file.txt | > test.txt", " ")

["cat","file.txt","|",">","test.txt"]
[0] = "cat"

struct s_data
{
	char *val;
	int type;
} t_data


 t_data cmnds[,,,,,,]

 vec_new(&v, 1, size)
vec_push
[000000000000000000000000000000000000000000000000000000000000]
 

*/
