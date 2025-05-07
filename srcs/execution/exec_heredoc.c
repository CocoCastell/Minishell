/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:23:16 by cochatel          #+#    #+#             */
/*   Updated: 2025/04/22 11:21:34 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	norminette_quote(char *input, t_shell *sh, bool quoted, int pipe_fd[2])
{
	char	*tmp;

	if (!quoted)
	{
		tmp = handle_d_q_here(input, sh);
		write(pipe_fd[1], tmp, ft_strlen(tmp));
		write(pipe_fd[1], "\n", 1);
		free_wrap(tmp);
	}
	else
	{
		write(pipe_fd[1], input, ft_strlen(input));
		write(pipe_fd[1], "\n", 1);
	}
}

void	heredoc_loop(int pipe_fd[2], char *hd, bool quoted, t_shell *sh)
{
	char	*input;
	int		output_fd;

	while (1)
	{
		output_fd = dup(STDOUT_FILENO);
		dup2(sh->std[1], STDOUT_FILENO);
		input = readline("heredoc> ");
		dup2(output_fd, STDOUT_FILENO);
		if (!input)
		{
			ft_printf(BL"msh: warning: heredoc delimited by end-of-file\n"DEF);
			break ;
		}
		if (ft_strncmp(input, hd, ft_strlen(hd) + 1) == 0)
			break ;
		norminette_quote(input, sh, quoted, pipe_fd);
		free_wrap(input);
	}
	free_wrap(input);
}

void	handle_heredoc_child(int pipe_fd[2], char **hd, t_shell *sh)
{
	int		i;
	bool	quoted;
	char	*clean_hd;

	i = 0;
	signal(SIGINT, signal_handle);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	while (hd[i])
	{
		quoted = is_quoted(hd[i]);
		clean_hd = strip_quotes(hd[i]);
		heredoc_loop(pipe_fd, clean_hd, quoted, sh);
		free_wrap(clean_hd);
		i++;
	}
	ft_free_string_array(hd);
	close(pipe_fd[1]);
	exit(0);
}

void	handle_heredoc_parent(int pipe_fd[2], pid_t pid, int status, char **hd)
{
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	ft_free_string_array(hd);
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

int	handle_heredoc(char *args, t_shell *sh)
{
	pid_t	pid;
	char	**hd;
	int		pipe_fd[2];
	int		status;

	hd = ft_split(args, '\n');
	status = 0;
	if (pipe(pipe_fd) == -1)
		return (perror("Pipe error"), 1);
	pid = fork();
	if (pid == 0)
		handle_heredoc_child(pipe_fd, hd, sh);
	else
		handle_heredoc_parent(pipe_fd, pid, status, hd);
	return (0);
}
