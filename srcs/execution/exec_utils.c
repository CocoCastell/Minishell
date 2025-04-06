/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:14:16 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/27 17:58:35 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*find_path(char **path, char *command)
{
	char	*full_path;	
	char	*half_path;
	int		i;

	i = -1;
	while (path[++i] != NULL)
	{
		half_path = ft_strjoin(path[i], "/");
		if (half_path == NULL)
		{
			ft_free_string_array(path);
			exit_error("Error: null pointer", 1);
		}
		full_path = ft_strjoin(half_path, command);
		free(half_path);
		if (full_path == NULL)
		{
			ft_free_string_array(path);
			exit_error("Error: null pointer", 1);
		}
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

void	*get_path(char *command, char **envp)
{
	char	**path;
	char	*full_path;
	int		i;

	i = 0;
	if (access(command, F_OK | X_OK) == 0)
		return (command);
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	if (path == NULL)
	{
		ft_free_string_array(path);
		exit_error("Error: null pointer", 1);
	}
	full_path = find_path(path, command);
	ft_free_string_array(path);
	return (full_path);
}

void	ft_sorpresa(void)
{
	printf("Ronaldo\n");
}
