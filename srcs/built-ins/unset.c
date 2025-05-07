/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:20:54 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/22 11:20:59 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Helper to initialize variables for unset function
 * @param u t_unset struct
 * @param sh t_shell struct
 * @param v var name (char *) being split
 * @return -1 if error, 1 if done
 */
int	init_unset_vars(t_unset *u, t_shell *sh, char *var)
{
	if (!var || !var[0] || !sh)
		return (-1);
	u->v = split_var_name(var);
	if (!u->v)
		return (-1);
	if (!ft_isinenv(sh->env, u->v))
		return (free_wrap(u->v), -1);
	u->len = ft_strlen(u->v);
	u->i = ft_arrlen(sh->env);
	u->env_cpy = (char **)malloc((u->i) * sizeof(char *));
	if (u->env_cpy == NULL)
		return (free_wrap(u->v), perror("Error on env memory alloc"), -1);
	u->i = -1;
	u->j = 0;
	return (1);
}

/**
 * @brief Handles the export, with or without vars
 * @param sh shell struct
 * @param vars array of strings being added. 
 * @return -1 if error, 0 if success.
 */
int	ft_handle_unset(t_shell *sh, char **vars)
{
	size_t	i;

	if (!sh)
		return (-1);
	sh->error = 1;
	if (ft_arrlen(vars) == 1)
	{
		sh->error = 0;
		return (sh->error);
	}
	i = 1;
	while (vars[i] != NULL)
	{
		if (ft_unset(sh, vars[i]) > -1)
			sh->error = 0;
		i++;
	}
	return (sh->error);
}

/**
 * @brief To delete specified variable from the env
 * @param sh shell struct
 * @param var string being deleted. Does NOT free the var passed.
 * @return -1 if error, 0 if updated, 1 if not in env
 */
int	ft_unset(t_shell *sh, char *var)
{
	t_unset	u;

	if (init_unset_vars(&u, sh, var) == -1)
		return (1);
	if (!ft_isinenv(sh->env, u.v))
		return (free_wrap(u.v), 1);
	while (sh->env[++u.i])
	{
		if (ft_strncmp(sh->env[u.i], u.v, u.len) == 0 && \
		sh->env[u.i][u.len] == '=')
			continue ;
		else
		{
			u.env_cpy[u.j] = ft_strdup(sh->env[u.i]);
			if (!u.env_cpy[u.j])
				return (ft_free_string_array(u.env_cpy), free(u.v), \
				perror("Error when duplicating env value"), 1);
			u.j++;
		}
	}
	u.env_cpy[u.j] = NULL;
	ft_free_string_array(sh->env);
	sh->env = u.env_cpy;
	return (free(u.v), 0);
}
