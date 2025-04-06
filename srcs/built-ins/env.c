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
		return (-1);
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
 * @param var string being searched for
 * @return variable value (char *) or NULL if not found 
 */
char	*ft_getenv(char **env, char *var)
{
	size_t		i;
	size_t		len;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
		{
			return (ft_strdup(&env[i][len + 1]));
		}
		i++;
	}
	return (NULL);
}

/**
 * @brief To update a specific env variable value
 * @param sh shell struct
 * @param var string being updated
 * @return -1 if error, 0 if no updates, 1 if updated
 */
int	ft_update_env(t_shell *sh, char *var)
{
	size_t	i;
	char	*v;
	size_t	len;

	if (verify_var(sh, var) == -1)
		return (-1);
	v = split_var_name(var);
	if (!v)
		return (-1);
	len = ft_strlen(v);
	i = -1;
	while (sh->env[++i])
	{
		if (ft_strncmp(sh->env[i], v, len) == 0 && sh->env[i][len] == '=')
		{
			free(sh->env[i]);
			sh->env[i] = ft_strdup(var);
			return (free(v), 1);
		}
	}
	return (free(v), 0);
}

/**
 * @brief To eset a variable on the env
 * @param sh shell struct
 * @param var string being added
 * @return -1 if error, 0 if added, 1 if updated
 */
int	ft_export(t_shell *sh, char *var)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	j = -1;
	if (verify_var(sh, var) == -1)
		return (-1);
	if (!ft_getenv(sh->env, var))
	{
		while (sh->env[i] != NULL)
			i++;
		new_env = (char **)malloc((i + 2) * sizeof(char *));
		if (new_env == NULL)
			return (perror("Error when allocating env memory"), -1);
		while (++j < i)
			new_env[j] = ft_strdup(sh->env[j]);
		new_env[i] = ft_strdup(var);
		new_env[++i] = NULL;
		ft_free_string_array(sh->env);
		sh->env = new_env;
	}
	else
		return (ft_update_env(sh, var));
	return (0);
}

/**
 * @brief To delete specified variable from the env
 * @param sh shell struct
 * @param var string being deleted
 * @return -1 if error, 1 if updated
 */
int	ft_unset(t_shell *sh, char *var)
{
	t_unset	u;

	if (init_unset_vars(&u, sh, var) == -1)
		return (-1);
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
				perror("Error when duplicating env value"), -1);
			u.j++;
		}
	}
	u.env_cpy[u.j] = NULL;
	ft_free_string_array(sh->env);
	sh->env = u.env_cpy;
	return (free(u.v), 1);
}
