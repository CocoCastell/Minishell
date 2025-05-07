/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:20:44 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/22 11:20:47 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Helper to print export env
 * @return -1 
 */
int	print_export(t_shell *sh)
{
	size_t	i;

	i = 0;
	while (sh->env[i])
	{
		ft_printf("declare -x %s\n", sh->env[i]);
		i++;
	}
	sh->error = 0;
	return (0);
}

/**
 * @brief Helper to init export vars
 * @return 1 on success, -1 on error
 */
int	init_export_vars(t_export_vars *ex, t_shell *sh, char *var)
{
	ex->i = 0;
	ex->j = -1;
	if (verify_var(sh, var) == -1)
		return (-1);
	ex->v = split_var_name(var);
	ex->append = is_valid_var_name(var);
	while (sh->env[ex->i] != NULL)
		ex->i++;
	return (1);
}

/**
 * @brief Handles the export, with or without vars
 * @param sh shell struct
 * @param vars array of strings being added. 
 * @return 1 if error, 0 if success.
 */
int	ft_handle_export(t_shell *sh, char **vars)
{
	size_t		i;
	static int	error = 0;

	if (!sh)
		return (1);
	error = 0;
	if (ft_arrlen(vars) == 1)
		return (print_export(sh));
	i = 1;
	while (vars[i] != NULL)
	{
		if (ft_export(sh, vars[i]) == 1)
			error = 1;
		i++;
	}
	return (error);
}

/**
 * @brief To set a variable on the env
 * @param sh shell struct
 * @param var string being added. Does NOT free the var passed. 
 * @return 1 if error, 0 if added / updated
 */
int	ft_export(t_shell *sh, char *var)
{
	t_export_vars	ex;

	if (init_export_vars(&ex, sh, var) == -1)
		return (sh->error);
	if (!ex.append)
	{
		if (!ft_isinenv(sh->env, ex.v))
		{
			ex.new_env = (char **)malloc((ex.i + 2) * sizeof(char *));
			if (ex.new_env == NULL)
				return (free_wrap(ex.v), perror("Error allocating memory"), 1);
			while (++ex.j < ex.i)
				ex.new_env[ex.j] = ft_strdup(sh->env[ex.j]);
			ex.new_env[ex.i] = ft_strdup(var);
			ex.new_env[++ex.i] = NULL;
			ft_free_string_array(sh->env);
			sh->env = ex.new_env;
		}
		else
			return (free_wrap(ex.v), ft_update_env(sh, var));
	}
	else
		return (free_wrap(ex.v), ft_append_env(sh, var));
	return (free_wrap(ex.v), 0);
}
