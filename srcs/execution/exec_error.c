/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:53 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/14 16:00:15 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_error(char *msg, int status)
{
	if (msg)
		perror(msg);
	exit (status);
}

void	free_all(t_shell *sh, int pipe_fd[2], char *msg, bool in_pipe)
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (msg != NULL)
		perror(msg);
	if (in_pipe == true)
	{
		ft_free_string_array(sh->env);
		free_sh(sh);
		exit(1);
	}
	else
		sh->error = 1;
}
