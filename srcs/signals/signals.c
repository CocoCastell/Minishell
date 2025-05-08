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

/**
 * @brief Handles the signals during blocking commands
 * @param signum 
 * @return void
 */
void	handle_fork_sig(int signum)
{
	if (signum == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
	if (signum == SIGINT)
		write(1, "\n", 1);
}

/**
 * @brief Sets the SIGINT for heredoc
 * @param signum 
 * @return void
 */
void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

/**
 * @brief Sets the SIGINT for heredoc
 * @param signum 
 * @return void
 */
void	signal_handler_2(int sig)
{
	if (sig == SIGINT)
		write(1, "^C\n", 4);
}

/**
 * @brief Sets the SIGINT for heredoc
 * @param signum 
 * @return void
 */
void	sig_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
	}
}

/**
 * @brief Sets the SIGINT & SIGQUIT handlers for the main execution
 * @param void
 * @return void
 */
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
