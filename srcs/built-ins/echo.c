/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:58:29 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/14 13:53:01 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Checks if argument is -n flag
 * @param str argument to check
 * @return 1 if flag, 0 if not
 */
int	is_flag_n(char *str)
{
	size_t	i;

	i = 0;
	if (!str || (str[i++] && str[i] != 'n'))
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Creates the string with all the arguments to print
 * @param e t_echo struct
 * @param args arguments to iterate
 * @return void
 */
void	loop_args(t_echo *e, char **args)
{
	while (args[e->i])
	{
		if (e->tmp)
		{
			e->new_tmp = ft_strjoin(e->tmp, " ");
			free(e->tmp);
			e->tmp = e->new_tmp;
			if (!e->tmp)
				break ;
			e->new_tmp = ft_strjoin(e->tmp, args[e->i]);
			free(e->tmp);
			e->tmp = e->new_tmp;
			if (!e->tmp)
				break ;
		}
		else
		{
			e->tmp = ft_strjoin("", args[e->i]);
			if (!e->tmp)
				break ;
		}
		e->i++;
	}
}

/**
 * @brief Function to recreate echo's behaviour. checks for -n flag to truncate 
 * @param args command arguments to echo
 * @write() and not printf() to avoid buffering which cause problem when no \n
 * @return 0
 */
int	ft_echo(char **args)
{
	t_echo	e;

	e.i = 1;
	e.tmp = NULL;
	e.new_tmp = NULL;
	e.new_line = 1;
	if (!args || !args[1])
		return (ft_printf("\n"), 0);
	if (is_flag_n(args[1]))
	{
		e.new_line = 0;
		while (args[e.i] && is_flag_n(args[e.i]))
			e.i++;
	}
	loop_args(&e, args);
	if (e.tmp)
	{
		write(1, e.tmp, ft_strlen(e.tmp));
		if (e.new_line)
			ft_printf("\n");
		free(e.tmp);
	}
	return (0);
}
