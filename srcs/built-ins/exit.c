/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:23:58 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/05 18:28:52 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit(t_node *tree, t_shell *sh, int *error)
{
	char	*arg;
	int		i;

	i = 0;

	arg = ft_strdup(tree->command.args[1]);
	while (arg != NULL && arg[i] != '\0')
	{	
		if (ft_isdigit(arg[i]) != 1)
		{
			sh->error = 2;
			printf("msh: t: Illegal number: %s\n", arg);
			return (free_wrap(arg), 1);
		}
		i++;
	}
	if (arg == NULL)
		*error = 0;
	else
		*error = ft_atoi(arg);
	ft_free_string_array(sh->env);
	return (free_sh(sh), free_wrap(arg), 0);
}
