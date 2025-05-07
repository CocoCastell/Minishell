/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_x.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:05:53 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/14 15:52:19 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Helper to update the variable
 * @param sh shell struct
 * @param a t_append struct
 * @param var string being appended. 
 * @return 1 if appended, 0 if not found
 */
static int	update_existing_env(t_shell *sh, t_append *a, char *var)
{
	while (sh->env[++a->i])
	{
		if (ft_strncmp(sh->env[a->i], a->v, a->l) == 0
			&& sh->env[a->i][a->l] == '=')
		{
			a->tmp = ft_strjoin(sh->env[a->i], var + (a->l + 2));
			free_wrap(sh->env[a->i]);
			sh->env[a->i] = ft_strdup(a->tmp);
			return (free_wrap(a->v), free_wrap(a->ap), free_wrap(a->tmp), 1);
		}
	}
	return (0);
}

/**
 * @brief To append to or add a variable on the env
 * @param sh shell struct
 * @param var string being appended. 
 * @return 1 if error, 0 if added / updated
 */
int	ft_append_env(t_shell *sh, char *var)
{
	t_append	a;

	if (verify_var(sh, var) == -1)
		return (sh->error);
	a.v = split_var_name(var);
	if (!a.v)
		return (1);
	a.n = ft_strtrim(a.v, "+");
	free_wrap(a.v);
	if (!a.n)
		return (1);
	a.v = ft_strdup(a.n);
	free_wrap(a.n);
	if (!a.v)
		return (1);
	a.l = ft_strlen(a.v);
	a.ap = ft_getenv(sh->env, a.v);
	a.i = -1;
	if (update_existing_env(sh, &a, var))
		return (0);
	a.tmp = ft_strjoin(a.v, var + (a.l + 1));
	a.err = ft_export(sh, a.tmp);
	return (free_wrap(a.v), free_wrap(a.ap), free(a.tmp), a.err);
}
// /**
//  * @brief To update a specific env variable value.
//  * @param sh shell struct
//  * @param var string being updated. Does NOT free the var passed.
//  * @return -1 if error, 0 if no updates, 1 if updated
//  */
// int	ft_append_env(t_shell *sh, char *var)
// {
// 	t_append	a;

// 	if (verify_var(sh, var) == -1)
// 		return (-1);
// 	a.v = split_var_name(var);
// 	if (!a.v)
// 		return (-1);
// 	a.v = ft_strtrim(a.v, "+");
// 	a.l = ft_strlen(a.v);
// 	a.ap = ft_getenv(sh->env, a.v);
// 	a.i = -1;
// 	while (sh->env[++a.i])
// 	{
// 		if (ft_strncmp(sh->env[a.i], a.v, a.l) == 0 && sh->env[a.i][a.l] == '=')
// 		{
// 			a.tmp = ft_strjoin(sh->env[a.i], var + (a.l + 2));
// 			free(sh->env[a.i]);
// 			sh->env[a.i] = ft_strdup(a.tmp);
// 			return (free_wrap(a.v), free_wrap(a.ap), free_wrap(a.tmp), 1);
// 		}
// 	}
// 	sh->error = 0;
// 	a.tmp = ft_strjoin(a.v, var + (a.l + 1));
// 	return (free(a.v), free_wrap(a.ap), ft_export(sh, a.tmp), free(a.tmp), 1);
// }
