/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:57:21 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/25 19:57:24 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Gets current working directory full path
 * @return current working directory, or NULL and err msg
 */
char	*ft_getpwd(void)
{
	static char	cwd[2000];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd() error"), NULL);
	return (cwd);
}

/**
 * @brief Gets current working directory shortened path
 * @return current working directory, or NULL and err msg
 */
char	*ft_getpwd_s(void)
{
	static char	cwd[2000];
	char		*last_slash;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd() error"), NULL);
	last_slash = ft_strrchr(cwd, '/');
	if (last_slash != NULL && *(last_slash + 1) != '\0')
		return (last_slash + 1);
	return (cwd);
}

/**
 * @brief Recreates cd functionality. Uses chdir() to check path.
 * @param path string representind the desired path
 * @param sh t_shell struct
 * @return 0 when done. On error, -1 is returned, and errno is set 
 * appropriately. 
 */
int	ft_cd(char *path, t_shell *sh)
{
	char	*curr_pwd;

	if (!sh)
		return (-1);
	curr_pwd = ft_strdup(ft_getpwd());
	if (!path || path[0] == '\0')
		return (set_dir_home(sh, curr_pwd));
	if (path[0] == '-' && path[1] == '\0')
		return (set_dir_prev(sh, curr_pwd));
	if (chdir(path) == -1)
		return (set_dir_error(sh, path, curr_pwd));
	return (set_dir_path(sh, curr_pwd, NULL));
}
