/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:05:53 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/29 19:05:55 by sluterea         ###   ########.fr       */
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
	if (!sh || !sh->env || !v || !ft_strchr(v, '='))
		return (-1);
	if (v[0] == '=')
		return (ft_printf("minishell: export: : bad variable name\n"), -1);
	return (1);
}

/**
 * @brief Create an env copy for the current Shell
 * @param env env being copied
 * @return NULL or the env copy
 */
char	**ft_create_env(char **env)
{
	char	**env_cpy;
	size_t	i;
	size_t	j;

	j = 0;
	i = ft_arrlen(env);
	env_cpy = (char **)malloc((i + 1) * sizeof(char *));
	if (env_cpy == NULL)
		return (perror("Error when allocating env memory"), NULL);
	while (j < i)
	{
		env_cpy[j] = ft_strdup(env[j]);
		if (env_cpy[j] == NULL)
			return (perror("Error when copying env var"), NULL);
		j++;
	}
	env_cpy[i] = NULL;
	return (env_cpy);
}

/**
 * @brief Helper to initialize variables for unset function
 * @param u t_unset struct
 * @param sh t_shell struct
 * @param v var name (char *) being split
 * @return -1 if error, 1 if done
 */
int	init_unset_vars(t_unset *u, t_shell *sh, char *var)
{
	u->v = split_var_name(var);
	if (!u->v)
		return (-1);
	u->len = ft_strlen(u->v);
	u->i = ft_arrlen(sh->env);
	u->env_cpy = (char **)malloc((u->i) * sizeof(char *));
	if (u->env_cpy == NULL)
		return (perror("Error when allocating env memory"), -1);
	u->i = -1;
	u->j = 0;
	return (1);
}
