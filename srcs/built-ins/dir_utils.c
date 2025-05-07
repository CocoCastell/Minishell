/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:38:07 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/10 20:29:43 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_dir_error(char *path, char *curr_pwd)
{
	if (errno == ENOENT)
		ft_printf("cd: no such file or directory: %s\n", path);
	else if (errno == EACCES)
		ft_printf("cd: permission denied: %s\n", path);
	else if (errno == ENOTDIR)
		ft_printf("cd: not a directory: %s\n", path);
	else
		ft_printf("cd: %s: %s\n", strerror(errno), path);
	return (free(curr_pwd), 1);
}

int	set_dir_path(t_shell *sh, char *curr_pwd, char *new)
{
	t_sd	s_d;

	s_d.tmp = NULL;
	if (!new)
	{
		s_d.set_new = ft_strjoin("PWD=", ft_getpwd());
		ft_update_env(sh, s_d.set_new);
	}
	else
	{
		if (chdir(new) != -1)
		{
			s_d.tmp = ft_strdup(new);
			s_d.set_new = ft_strjoin("PWD=", s_d.tmp);
			free_multiple(2, new, s_d.tmp, NULL);
			ft_update_env(sh, s_d.set_new);
		}
		else
			return (set_dir_error(new, curr_pwd), free(new), -1);
	}
	s_d.set_old = ft_strjoin("OLDPWD=", curr_pwd);
	ft_update_env(sh, s_d.set_old);
	sh->dir = ft_getpwd();
	return (free(curr_pwd), free(s_d.set_old), free(s_d.set_new), 0);
}

int	set_dir_prev(t_shell *sh, char *curr_pwd)
{
	char	*old_pwd;

	old_pwd = ft_getenv(sh->env, "OLDPWD");
	if (!old_pwd)
		return (-1);
	return (set_dir_path(sh, curr_pwd, old_pwd));
}

/**
 * @brief Helper to set directory to home.
 * @param sh t_shell struct
 * @param curr_pwd string representind the current working path
 * @return 0 when done. On error, -1 is returned, and errno is set correctly.
 */
int	set_dir_home(t_shell *sh, char *curr_pwd)
{
	char	*home;
	char	*tmp;
	char	*tmpenv;

	tmp = ft_strdup("/home/");
	if (!tmp)
		return (1);
	tmpenv = ft_getenv(sh->env, "USER");
	home = ft_strjoin(tmp, tmpenv);
	free_multiple(2, tmp, tmpenv, NULL);
	if (!home)
		return (1);
	return (set_dir_path(sh, curr_pwd, home));
}
