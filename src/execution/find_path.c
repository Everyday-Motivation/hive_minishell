/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:51:38 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/02 15:23:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	res = malloc(len);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcpy(res, s1, len);
	ft_strlcat(res, s2, len);
	ft_strlcat(res, s3, len);
	return (res);
}

char	*get_path_env(t_vec *env)
{
	size_t	j;
	char	**entry;

	j = 0;
	while (j < env->len)
	{
		entry = (char **)ft_vec_get(env, j);
		if (strncmp(*entry, "PATH=", 5) == 0)
			return (*entry + 5);
		j++;
	}
	return (NULL);
}

char	*find_executable_in_paths(char *path_env, char *cmd)
{
	char	**paths;
	char	*full_path;
	size_t	i;

	paths = ft_split(path_env, ':');
	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_strjoin_3(paths[i++], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_split(paths);
	return (NULL);
}

char	*search_path(char *cmd, t_vec *env)
{
	char	*path_env;

	path_env = get_path_env(env);
	if (!path_env)
		return (NULL);
	return (find_executable_in_paths(path_env, cmd));
}
