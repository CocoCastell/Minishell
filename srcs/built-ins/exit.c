/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:23:58 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 16:01:19 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_atoi_exit(const char *str)
{
	int	i;
	int	sign;
	int	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign *= -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (2);
	return (nb * sign);
}

int	check_numeric_arg(char *arg, int *error)
{
	int	i;

	if (arg == NULL)
		return (0);
	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg != NULL && arg[i] != '\0')
	{
		if (ft_isdigit(arg[i]) != 1)
		{
			*error = 2;
			ft_printf("msh: exit: %s: numeric argument required\n", arg);
			return (1);
		}
		i++;
	}
	return (0);
}

int	handle_exit_args(t_node *tree, char **arg, int *error)
{
	int	arg_count;

	arg_count = 0;
	if (tree->command.args)
	{
		while (tree->command.args[arg_count] != NULL)
			arg_count++;
	}
	if (arg_count > 1)
	{
		*arg = ft_strdup(tree->command.args[1]);
		if (!*arg)
			return (1);
		if (check_numeric_arg(*arg, error) == 1)
			return (0);
	}
	if (arg_count > 2)
	{
		ft_printf("msh: exit: too many arguments\n");
		return (1);
	}
	return (0);
}

int	ft_exit(t_node *tree, t_shell *sh, int *error)
{
	char	*arg;
	int		ret;

	arg = NULL;
	ft_printf("exit\n");
	ret = handle_exit_args(tree, &arg, error);
	if (ret != 0)
		return (free_wrap(arg), ret);
	if (arg == NULL)
		*error = 0;
	else
		*error = ft_atoi_exit(arg);
	ft_free_string_array(sh->env);
	rl_clear_history();
	return (free_sh(sh), free_wrap(arg), 0);
}
