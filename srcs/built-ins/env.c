/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:43:27 by sluterea          #+#    #+#             */
/*   Updated: 2025/03/26 13:43:29 by sluterea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
 * @brief Prints the env variables
 * @param args command arg (no args, no options, expects only "env" (1))
 * @param sh shell struct
 * @return -1 if wrong args, 0 on finish
 */
int	ft_env(char **args, t_shell *sh)
{
	size_t	i;

	i = 0;
	if (ft_arrlen(args) != 1)
	{
		ft_printf("env: %s: no such file or directory\n", args[1]);
		return (127);
	}
	while (sh->env[i])
	{
		ft_printf("%s\n", sh->env[i]);
		i++;
	}
	return (0);
}

/**
 * @brief To get the value of the variable looked for
 * @param env environment array (sh->env)
 * @param var string being searched for. Does NOT free/modify 
 * the var passed.
 * @return new value (char *) or NULL if not found 
 */
char	*ft_getenv(char **env, char *var)
{
	size_t		i;
	size_t		len;
	char		*str;

	len = ft_strlen(var);
	i = 0;
	str = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
		{
			str = ft_strdup(&env[i][len + 1]);
			if (!str)
				return (perror("alloc error"), NULL);
			return (str);
		}
		i++;
	}
	return (NULL);
}

/**
 * @brief To update a specific env variable value.
 * @param sh shell struct
 * @param var string being updated. Does NOT free the var passed.
 * @return -1 if error, 0 if no updates/ updated
 */
int	ft_update_env(t_shell *sh, char *var)
{
	size_t	i;
	char	*v;
	size_t	len;

	if (verify_var(sh, var) == -1)
		return (sh->error);
	v = split_var_name(var);
	if (!v)
		return (1);
	len = ft_strlen(v);
	i = -1;
	while (sh->env[++i])
	{
		if (ft_strncmp(sh->env[i], v, len) == 0 && sh->env[i][len] == '=')
		{
			free_wrap(sh->env[i]);
			sh->env[i] = ft_strdup(var);
			return (free_wrap(v), 0);
		}
	}
	sh->error = 0;
	return (free_wrap(v), 0);
}
