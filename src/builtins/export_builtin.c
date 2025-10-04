/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:50 by timurray          #+#    #+#             */
/*   Updated: 2025/10/04 18:07:37 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	valid_export(char *line)
{
	size_t	i;
	size_t	len;
	int		has_equals;

	if (!line || !((ft_isalpha(line[0])) || line[0] == '_'))
		return (0);
	i = 0;
	has_equals = 0;
	len = ft_strlen(line);
	while ((i < len))
	{
		if (!((ft_isalnum(line[i])) || line[i] == '=' || line[i] == '_'
				|| line[i] == ' '))
			return (0);
		if (!has_equals && ft_isspace(line[i]))
			return (0);
		if (line[i] == '=')
			has_equals = 1;
		i++;
	}
	if (has_equals)
		return (1);
	return (0);
}

char	*get_key_val(char *line)
{
	size_t	i;

	i = 0;
	while (!(line[i] == '='))
		i++;
	return (ft_substr(line, 0u, i + 1));
}

char	*get_valid_entry(char *line)
{
	size_t	i;
	int		has_equals;

	i = 0;
	has_equals = 0;
	while (line[i])
	{
		if (line[i] == '=')
			has_equals = 1;
		if (has_equals && line[i] == ' ')
			break ;
		i++;
	}
	return (ft_substr(line, 0u, i));
}

void	sort_alpha(t_vec *v)
{
	size_t	i;
	size_t	j;
	char	*temp;
	char	**line;
	char	**line_comp;

	i = 0;
	while (i < v->len + 1)
	{
		j = i + 1;
		line = (char **)ft_vec_get(v, i);
		while (j < v->len)
		{
			line_comp = (char **)ft_vec_get(v, j);
			if (ft_strncmp(*line, *line_comp, ft_strlen(*line)) > 0)
			{
				temp = ft_strdup(*line);
				free(*line);
				*line = ft_strdup(*line_comp);
				free(*line_comp);
				*line_comp = strdup(temp);
				free(temp);
			}
			j++;
		}
		i++;
	}
}

int	bi_export(char **av, t_vec *env)
{
	size_t	i;
	size_t	index;
	char	*key;
	char	**line;
	char	*new_line;

	if (!av || !av[0])
	{
		sort_alpha(env);
		print_str_vec(env, "declare -x "); // Alphabetical printout
	}
	i = 0;
	while (av[i])
	{
		if (valid_export(av[i]))
		{
			key = get_key_val(av[i]);
			new_line = get_valid_entry(av[i]);
			if (!new_line)
				return (0);
			if (!str_in_str_vec(env, key))
				ft_vec_push(env, &new_line);
			else
			{
				index = get_str_index(env, key);
				line = (char **)ft_vec_get(env, index);
				free(*line);
				*line = new_line;
			}
			free(key);
		}
		i++;
	}
	ft_putendl_fd("\n\n",1);
	print_str_vec(env, "declare -x ");
	return (1);
}
/*

TODO: Alphabetical

TODO: no args shows all keys even with null values

TODO: Error message
	timurray@c1r1p5:~/Documents/projects$ export 111=wwwwwww
	bash: export: `111=wwwwwww': not a valid identifier
 */