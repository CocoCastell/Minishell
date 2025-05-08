/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:23:16 by cochatel          #+#    #+#             */
/*   Updated: 2025/05/07 17:48:29 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_heredoc(t_here *hd, char *delimiter)
{
	hd->quoted = is_quoted(delimiter);
	hd->clean_delim = strip_quotes(delimiter);
	if (pipe(hd->fd) == -1)
		return (-1);
	hd->pid = fork();
	if (hd->pid == -1)
	{
		close(hd->fd[0]);
		close(hd->fd[1]);
		return (-1);
	}
	return (1);
}

void	write_while_loop(t_here *hd, t_shell *sh)
{
	char	*expanded;

	if (!hd->quoted)
	{
		expanded = handle_d_q_here(hd->line, sh);
		write(hd->fd[1], expanded, ft_strlen(expanded));
		write(hd->fd[1], "\n", 1);
		free(expanded);
	}
	else
	{
		write(hd->fd[1], hd->line, ft_strlen(hd->line));
		write(hd->fd[1], "\n", 1);
	}
}

void	do_if_loop(t_here *hd, t_shell *sh, char *delimiter)
{
	while (1)
	{
		hd->line = readline("> ");
		if (hd->line == NULL)
		{
			free(hd->line);
			printf("msh: warning: here-document at line 1 delimited by");
			printf(" end-of-file (wanted `%s')\n", hd->clean_delim);
			break ;
		}
		if ((ft_strncmp(hd->line, hd->clean_delim, ft_strlen(hd->line)) == 0
				|| ft_strncmp(hd->line, delimiter, ft_strlen(hd->line)) == 0)
			&& (hd->line[0] != '\0'))
		{
			free(hd->line);
			break ;
		}
		write_while_loop(hd, sh);
		free(hd->line);
	}
}

int	handle_heredoc(char *delimiter, t_shell *sh)
{
	t_here			hd;
	void			(*old_sigint)(int);

	old_sigint = signal(SIGINT, signal_handler_2);
	if (init_heredoc(&hd, delimiter) == -1)
		return (-1);
	if (hd.pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(hd.fd[0]);
		do_if_loop(&hd, sh, delimiter);
		free(hd.clean_delim);
		close(hd.fd[1]);
		exit(0);
	}
	else
	{
		close(hd.fd[1]);
		waitpid(hd.pid, &hd.status, 0);
		signal(SIGINT, old_sigint);
		free(hd.clean_delim);
		if (WIFSIGNALED(hd.status) && WTERMSIG(hd.status) == SIGINT)
			return (close(hd.fd[0]), -2);
		return (hd.fd[0]);
	}
}
// void	norminette_quote(char *input, t_shell *sh, bool quoted, int pipe_fd[2])
// {
// 	char	*tmp;
// 	if (!quoted)
// 	{
// 		tmp = handle_d_q_here(input, sh);
// 		write(pipe_fd[1], tmp, ft_strlen(tmp));
// 		write(pipe_fd[1], "\n", 1);
// 		free_wrap(tmp);
// 	}
// 	else
// 	{
// 		write(pipe_fd[1], input, ft_strlen(input));
// 		write(pipe_fd[1], "\n", 1);
// 	}
// }
// void	heredoc_loop(int pipe_fd[2], char *hd, bool quoted, t_shell *sh)
// {
// 	char	*input;
// 	while (1)
// 	{
// 		input = readline("heredoc> ");
// 		if (!input)
// 		{
// 			ft_printf(BL"msh: warning: heredoc delimited by end-of-file\n"DEF);
// 			break ;
// 		}
// 		if (ft_strncmp(input, hd, ft_strlen(hd) + 1) == 0)
// 			break ;
// 		norminette_quote(input, sh, quoted, pipe_fd);
// 		free_wrap(input);
// 	}
// 	free_wrap(input);
// }
// void	handle_heredoc_child(int pipe_fd[2], char **hd, t_shell *sh)
// {
// 	int		i;
// 	bool	quoted;
// 	char	*clean_hd;
// 	i = 0;
// 	signal(SIGINT, signal_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	close(pipe_fd[0]);
// 	while (hd[i])
// 	{
// 		quoted = is_quoted(hd[i]);
// 		clean_hd = strip_quotes(hd[i]);
// 		heredoc_loop(pipe_fd, clean_hd, quoted, sh);
// 		free_wrap(clean_hd);
// 		i++;
// 	}
// 	ft_free_string_array(hd);
// 	close(pipe_fd[1]);
// 	exit(0);
// }
// void	handle_heredoc_parent(int pipe_fd[2], pid_t pid, int status, char **hd)
// {
// 	void	(*old_sigint)(int);
// 	void	(*old_sigquit)(int);
// 	old_sigint = signal(SIGINT, SIG_IGN);
// 	old_sigquit = signal(SIGQUIT, SIG_IGN);
// 	close(pipe_fd[1]);
// 	waitpid(pid, &status, 0);
// 	ft_free_string_array(hd);
// 	signal(SIGINT, old_sigint);
// 	signal(SIGQUIT, old_sigquit);
// 	dup2(pipe_fd[0], STDIN_FILENO);
// 	close(pipe_fd[0]);
// }
// int	handle_heredoc(char *args, t_shell *sh)
// {
// 	pid_t	pid;
// 	char	**hd;
// 	int		pipe_fd[2];
// 	int		status;
// 	hd = ft_split(args, '\n');
// 	status = 0;
// 	if (pipe(pipe_fd) == -1)
// 		return (perror("Pipe error"), 1);
// 	pid = fork();
// 	if (pid == 0)
// 		handle_heredoc_child(pipe_fd, hd, sh);
// 	else
// 		handle_heredoc_parent(pipe_fd, pid, status, hd);
// 	return (0);
// }
// int handle_heredoc(char *args, t_shell *sh)
// {
// 	pid_t	pid;
// 	char	**hd;
// 	int		pipe_fd[2];
// 	int		status;
// 	hd = ft_split(args, '\n');
// 	status = 0;
// 	if (pipe(pipe_fd) == -1)
// 		return (-1);
// 	pid = fork();
// 	if (pid == 0)
// 		handle_heredoc_child(pipe_fd, hd, sh);
// 	else
// 	{
// 		void (*old_sigint)(int) = signal(SIGINT, SIG_IGN);
// 		void (*old_sigquit)(int) = signal(SIGQUIT, SIG_IGN);
// 		close(pipe_fd[1]);
// 		waitpid(pid, &status, 0);
// 		ft_free_string_array(hd);
// 		signal(SIGINT, old_sigint);
// 		signal(SIGQUIT, old_sigquit);
// 		return pipe_fd[0];
// 	}
// 	return (-1);
// }
// #define HEREDOC_TMP_FILE "/tmp/minishell_heredocXXXXXX"
// static void	sig_heredoc(int sig)
// {
// 	if (sig == SIGINT)
//     {
//         write(1, "\n", 1);
//     }
// }
/* int	handle_heredoc(char *delimiter) */
/* { */
/* 	int		fd[2]; */
/* 	pid_t	pid; */
/* 	int		status; */
/* 	char	*line; */
/* 	bool	quoted; */
/* 	quoted = is_quoted(hd[i]); */
/* 	void (*old_sigint)(int) = signal(SIGINT, signal_handler_2); */
/* 	if (pipe(fd) == -1) */
/* 		return (-1); */
/* 	pid = fork(); */
/* 	if (pid == -1) */
/* 	{ */
/* 		close(fd[0]); */
/* 		close(fd[1]); */
/* 		return (-1); */
/* 	} */
/* 	if (pid == 0) */
/* 	{ */
/* 		signal(SIGINT, SIG_DFL); */
/* 		close(fd[0]); */
/* 		while (1) */
/* 		{ */
/* 			line = readline("> "); */
/* 			if (!line) */
/* 			{ */
/* 				free(line); */
/* 				printf("msh: warning: here-document at line 1 delimited by \
end-of-file (wanted `hey')\n"); */
/* 				break; */
/* 			} */
/* 			if (strcmp(line, delimiter) == 0) */
/* 			{ */
/* 				free(line); */
/* 				break; */
/* 			} */
/* 			write(fd[1], line, strlen(line)); */
/* 			write(fd[1], "\n", 1); */
/* 			free(line); */
/* 		} */
/* 		close(fd[1]); */
/* 		exit(0); */
/* 	} */
/* 	else // Parent process */
/*     { */
/* 		close(fd[1]); */
/* 		waitpid(pid, &status, 0); */
/* 		signal(SIGINT, old_sigint); */
/* 		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) */
/* 		{ */
/* 			close(fd[0]); */
/* 			return (130); */
/* 		} */
/* 		return fd[0]; */
/* 	} */
/* } */
