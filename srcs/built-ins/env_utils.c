/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:05:53 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/14 15:50:37 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Helper to check string array length
 * @param arr char ** to check
 * @return int (length)
 */
int	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

/**
 * @brief Helper to split the variable name from the value
 * @param var string being split
 * @return NULL or (char *) var name
 */
char	*split_var_name(char *var)
{
	size_t	i;
	char	*v;

	if (!var)
		return (NULL);
	i = 0;
	while (var[i] != '=' && var[i] != '\0')
		i++;
	v = malloc(i + 1);
	if (!v)
		return (NULL);
	ft_strlcpy(v, var, i + 1);
	return (v);
}

/**
 * @brief Helper to verify if the parameters are valid, specially the v name
 * @param sh t_shell struct
 * @param v var name (char *) being verified
 * @return -1 if error (and print if bad name), 1 if verified
 */
int	verify_var(t_shell *sh, char *v)
{
	if (!sh || !sh->env)
	{
		sh->error = 1;
		return (-1);
	}
	if (is_valid_var_name(v) == -1)
	{
		sh->error = 1;
		ft_printf("msh: export: `%s': not a valid identifier\n", v);
		return (-1);
	}
	if (!ft_strchr(v, '='))
	{
		sh->error = 0;
		return (-1);
	}
	return (0);
}

/**
 * @brief To get the value of the variable looked for
 * @param env environment array (sh->env)
 * @param var string being searched for. Does NOT free/modify 
 * the var passed.
 * @return 1 if found or 0 if not found 
 */
int	ft_isinenv(char **env, char *var)
{
	size_t		i;
	size_t		len;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Helper to verify if the env var name is valid,
 * @param v var name (char *) being verified
 * @return -1 if error, 1 if verified, 2 if flag
 */
int	is_valid_var_name(char *str)
{
	size_t	i;

	if (!str || !str[0])
		return (-1);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (-1);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
		{
			if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
				return (1);
			else
				return (-1);
		}
		i++;
	}
	return (0);
}
