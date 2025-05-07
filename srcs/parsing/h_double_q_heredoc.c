/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_double_q_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:22 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/10 20:41:52 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles retrieving of env variable.
 * @param line Input string. 
 * @param i iterator
 * @param sh t_shell Struct (env and error being used)
 * @return env var value
 */
char	*handle_env_var_h(char *line, int *i, t_shell *sh)
{
	t_env_h		eh;
	const char	*del = " |<>&$\"'-()\t\n\v\f\r";

	eh.var = NULL;
	eh.j = 0;
	if (line[eh.j] == '?')
	{
		eh.var = ft_itoa(sh->error);
		(*i)++;
	}
	else
	{
		while (!ft_strchr(del, line[eh.j]))
		{
			eh.j++;
			(*i)++;
		}
		eh.str = ft_substr(line, 0, eh.j);
		eh.var = ft_getenv(sh->env, eh.str);
		if (!eh.var)
			eh.var = ft_strdup("");
		free(eh.str);
	}
	return (eh.var);
}

char	*make_string(char *str, char *tmp)
{
	char	*new;

	new = ft_strjoin(str, tmp);
	free_wrap(str);
	free_wrap(tmp);
	return (new);
}

/**
 * @brief Handles the double quotes
 * @param line Input string. 
 * @param sh t_shell Struct
 * @return 1 on creation, -1 on error
 */
char	*handle_d_q_here(char *line, t_shell *sh)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = NULL;
	tmp = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			i++;
			tmp = handle_env_var_h(line + i, &i, sh);
			str = make_string(str, tmp);
		}
		else
		{
			tmp = ft_char_to_str(line[i]);
			str = make_string(str, tmp);
			i++;
		}
	}
	return (str);
}
