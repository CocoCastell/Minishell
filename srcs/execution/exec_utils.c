/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:14:16 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/06 20:06:21 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	change_in_out(t_node *cmd_tree, int is_builtin)
{
	if (cmd_tree->command.redir_in > -1)
	{
		if (dup2(cmd_tree->command.redir_in, STDIN_FILENO) == -1)
			return (printf("Dup2 error\n"), 1);
		if (is_builtin == false)
			close(cmd_tree->command.redir_in);
	}
	if (cmd_tree->command.redir_out > -1)
	{
		if (dup2(cmd_tree->command.redir_out, STDOUT_FILENO) == -1)
			return (printf("Dup2 error\n"), 1);
		if (is_builtin == false)
			close(cmd_tree->command.redir_out);
	}
	return (0);
}

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
	if (command[0] == '.' && command[1] == '/')
	{
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		return (NULL);
	}
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
	return (ft_free_string_array(path), full_path);
}

bool	is_quoted(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	return (len >= 2 && s[0] == '\'' && s[len - 1] == '\'');
}

char	*strip_quotes(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (is_quoted(s))
		return (ft_substr(s, 1, len - 2));
	return (ft_strdup(s));
}
