/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:57:21 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/10 20:40:26 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Gets current working directory full path
 * @param sh t_shell struct to use sh->dir
 * @return current working directory -1-, or err msg -0-
 */
int	ft_pwd(t_shell *sh)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL && sh->dir && sh->dir[0] != '\0')
	{
		if (errno == ENOENT)
		{
			ft_printf("%s\n", sh->dir);
			return (0);
		}
		perror("pwd");
		return (1);
	}
	else if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}

/**
 * @brief Gets current working directory full path
 * @return current working directory, or NULL and err msg
 */
char	*ft_getpwd(void)
{
	static char	cwd[5000];
	static int	count = 0;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		count = 0;
		return (cwd);
	}
	if (++count >= 10)
	{
		ft_printf("cd: failed too many times, trying to recover at root\n");
		if (chdir("/") == -1)
		{
			ft_printf("fatal: shell environment corrupted, exiting...\n");
			exit(1);
		}
		count = 0;
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			return (NULL);
		return (cwd);
	}
	if (chdir("..") == -1)
		return (ft_printf("cd: cannot move up to parent directory\n"), NULL);
	return (ft_getpwd());
}

int	cd_error(char **args, char *path)
{
	int	arg_nb;

	if (!args)
		return (-1);
	arg_nb = 0;
	while (args[arg_nb] != NULL)
		arg_nb++;
	if (arg_nb > 2)
		return (ft_printf("msh: cd: too many arguments\n"), -1);
	if (path != NULL && path[0] == '-' && path[1] && \
		path[1] == '-' && path[2] && path[2] != '\0')
		return (ft_printf("msh: cd: %s: invalid option\n", path), -1);
	if (path != NULL && path[0] == '-' && path[1] != '\0' && path[1] != '-')
		return (ft_printf("msh: cd: %s: invalid option\n", path), -1);
	return (0);
}

/**
 * @brief Recreates cd functionality. Uses chdir() to check path.
 * @param path string representind the desired path
 * @param args array of cd arguments
 * @param sh t_shell struct
 * @return 0 when done. On error, -1 is returned, and errno is set 
 * appropriately. 
 */
int	ft_cd(char *path, char **args, t_shell *sh)
{
	char	*curr_pwd;

	if (!sh || cd_error(args, path) == -1)
		return (1);
	curr_pwd = ft_strdup(sh->dir);
	if (!curr_pwd)
		return (1);
	if (!path || path[0] == '\0')
		return (set_dir_home(sh, curr_pwd));
	if (path[0] == '~' && path[1] == '\0')
		return (set_dir_home(sh, curr_pwd));
	if (path[0] == '-' && path[1] == '\0')
	{
		set_dir_prev(sh, curr_pwd);
		curr_pwd = ft_strdup(ft_getpwd());
		return (ft_printf("%s\n", curr_pwd), free(curr_pwd), 0);
	}
	if (path[0] == '-' && path[1] == '-' && path[2] == '\0')
		return (set_dir_home(sh, curr_pwd));
	if (chdir(path) == -1)
		return (set_dir_error(path, curr_pwd));
	return (set_dir_path(sh, curr_pwd, NULL));
}
