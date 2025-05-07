/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluterea <sluterea@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:12:31 by sluterea          #+#    #+#             */
/*   Updated: 2025/04/05 17:50:16 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_fork_sig(int signum)
{
	//write(1, "Fork sig\n", 10);
	if (signum == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
	if (signum == SIGINT)
		write(1, "\n", 1);
}

void	signal_handler(int sig)
{
	//write(1, "Hand sig\n", 10);
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	handle_others(int sig)
{
	//write(1, "Others\n", 10);
	if (sig == SIGINT)
	{
		write(1, "", 0);
	}
}

int	manage_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
/*
int	manage_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask); //se puede usar?
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (ft_printf("Error when receiving SIGINT\n"), -1);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (ft_printf("Error when ignoring SIGQUIT\n"), -1);
	return (0);
}
*/
/*
void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		write(STDOUT_FILENO, "\n", 1);
	}
}
*/
